#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#include <cstdlib>
#include <cmath>
#include <iostream>
#include <string>
#include <random>

using namespace std;

const float PI = 3.14159265f;
const int W = 1200, H = 720;
const float WORLD = 2400;
const int LEVEL_COUNT = 6;
const int BOSS_LEVEL = 5;
const float PORTAL_X = 2120;
const float RUNE_X = 1980;

struct RGB {
    float r, g, b;
};

struct Hero {
    const char* name;
    RGB shirt, hair;
};

const Hero heroes[] = {
    {"Scarlet", {.90f,.20f,.33f}, {.39f,.16f,.12f}},
    {"Han",     {.22f,.62f,.84f}, {.13f,.12f,.16f}},
    {"Keanu",   {.44f,.33f,.72f}, {.20f,.14f,.12f}},
    {"Ian",     {.95f,.62f,.16f}, {.28f,.19f,.12f}}
};

const char* difficulties[] = {
    "EASY", "NORMAL", "TRICKY", "HARD", "EXTREME", "BOSS"
};

const char* worlds[] = {
    "Briarwood Outskirts",
    "Abandoned Suburb",
    "Signal Woods",
    "Mirror Marsh",
    "Ashfall Ruins",
    "The Breach"
};

const char* supplies[] = {
    "forest herbs", "fuse parts", "signal shards",
    "glow caps", "ember stones", "rift fragments"
};

const RGB grounds[] = {
    {.12f,.29f,.24f}, {.24f,.36f,.34f}, {.23f,.27f,.39f},
    {.18f,.33f,.31f}, {.35f,.27f,.28f}, {.20f,.28f,.40f}
};

int active = 0, dimension = 0, completed = 0;
int coins = 0, boots = 0;

bool choosingCharacter = true;
int menuSelection = 0;
float menuTime = 0;

bool levelBriefing = false;
string briefingLines[6];

int taskType = 0, required = 3, collected = 0;
float px = 230, py = 160, cameraX = 0;
float worldTime = 0;
float runeAngle = 0, portalScale = .8f;
float targetAngle = 90, targetScale = 1;

bool taskDone = false, parcel = false, hasKey = false;
bool picked[6] = {};
bool held[256] = {};
bool arrows[4] = {};

const float nodeX[] = {470,760,1050,1380,1670,2240};
const float nodeY[] = {165,310,185,320,170,190};

float relicX = 1560, relicY = 200;
float flash = 0;
string notice;
float noticeUntil = 0;

int lastTick = 0;
int vx = 0, vy = 0, vw = W, vh = H, windowH = H;
GLuint terrainVBO = 0;

// Static world geometry stored in a VBO.
const GLfloat terrain[] = {
    0,0, WORLD,0, WORLD,365,
    0,0, WORLD,365, 0,365
};

float limit(float value, float low, float high) {
    return value < low ? low : (value > high ? high : value);
}

float distanceTo(float x, float y, float a, float b) {
    return sqrt((x-a)*(x-a) + (y-b)*(y-b));
}

float daylightAmount(float t) {
    return .5f + .5f*cos(2*PI*t/60);
}

// Timed translation: move for one second, pause for two seconds.
float keyX(float t) {
    int leg = static_cast<int>(t/3);
    float p = limit(t-leg*3, 0, 1);
    p = p*p*(3-2*p);
    return leg%2 == 0 ? 1950+300*p : 2250-300*p;
}

float keyY(float t) {
    return 345+7*sin(t*2);
}

bool tuned() {
    return fabs(runeAngle-targetAngle) < 1 &&
           fabs(portalScale-targetScale) < .015f;
}

void say(const string& message) {
    notice = message;
    noticeUntil = worldTime+7;
}

void clearInput() {
    for(int i=0; i<256; ++i) held[i] = false;
    for(int i=0; i<4; ++i) arrows[i] = false;
}

// ---------- Random traps ----------

struct Trap {
    float x, y;
    int type, phase;
    float remaining;
};

const int MAX_TRAPS = 10;
const int SECTORS = 5;

Trap traps[MAX_TRAPS];
int trapCount = 0;
int safeLane[SECTORS];
float sectorX[SECTORS];
const float lanes[] = {145,230,315};

mt19937 levelRandom(random_device{}());

int randomInt(int low, int high) {
    return uniform_int_distribution<int>(low,high)(levelRandom);
}

void generateObstacles() {
    const float centers[] = {550,820,1030,1590,1850};
    trapCount = 0;

    for(int section=0; section<SECTORS; ++section) {
        sectorX[section] = centers[section]+randomInt(-18,18);
        safeLane[section] = randomInt(0,2);
        bool added = false;

        for(int lane=0; lane<3; ++lane) {
            if(lane == safeLane[section]) continue;
            if(added && randomInt(0,99) > 45+dimension*10) continue;

            int type = dimension == 0 ? 3 :
                       randomInt(0, dimension >= 2 ? 3 : 1);

            traps[trapCount++] = {
                sectorX[section], lanes[lane], type, 0, 0
            };
            added = true;
        }
    }
}

// ---------- Player, ghost, boss, guardian ----------

int health = 3, deaths = 0;
float immunity = 0, dashTime = 0, dashCooldown = 0;
float facingX = 1, facingY = 0, dashX = 1, dashY = 0;
float checkpointX = 230, checkpointY = 160;
bool checkpointSet = false;

float hurtFlash = 0;
float wispX = 1580, wispY = 240;
bool ghostAwake = false;

const float BOSS_X = 1810, BOSS_Y = 270;
const int BOSS_MAX_HP = 100;
const int MAGIC_COST = 40;
const int MAX_SHOTS = 64;
const float CLOSE_SPELL_RANGE = 100;
const float MAX_SPELL_RANGE = 430;

struct Shot {
    float x, y, dx, dy;
    bool active;
};

Shot shots[MAX_SHOTS];

bool magicOwned = false;
int magicDebt = 0;
int bossHP = 0;

float attackCooldown = 0, castTime = 0;
float bossTimer = 1.6f, bossHurt = 0;
float barrageTimer = 4, spitFlash = 0;
float spellEndX = 0, spellEndY = 0;
float damageTextTime = 0;
int lastSpellDamage = 0;

const int GUARDIAN_COST = 50;
bool guardianOwned = false;
float guardianTimer = 0, guardianGlow = 0;
float angelX = 185, angelY = 295;

int spellDamage(float range) {
    if(range > MAX_SPELL_RANGE) return 0;
    return range <= CLOSE_SPELL_RANGE ? 10 : 5;
}

float guardianInterval() {
    if(dimension == 3) return 7;
    if(dimension == 4) return 5;
    if(dimension == 5) return 4;
    return 0;
}

void resetGuardian() {
    guardianTimer = guardianInterval();
    guardianGlow = 0;
    angelX = px-45;
    angelY = py+135;
}

// ---------- Guide and conversations ----------

enum GuidePhase {
    GUIDE_APPROACH,
    GUIDE_BRIEFING,
    GUIDE_RETURN,
    GUIDE_PATROL
};

GuidePhase guidePhase = GUIDE_APPROACH;
bool guideGiven = false;

int npcHero = 1;
float npcX = 200, npcY = 285, npcClock = 0;

bool talking = false, pendingDelivery = false;
int dialoguePage = 0;
string dialogue[3];

string objective();
void completeLocalTask();
void openLevelBriefing();

void resetPortalGauntlet(bool newLayout);
void updatePortalGauntlet(float dt);
void drawPortalGauntlet();

void updateNPC(float dt) {
    if(talking || guidePhase == GUIDE_BRIEFING) return;

    if(guidePhase == GUIDE_APPROACH || guidePhase == GUIDE_RETURN) {
        float targetX = guidePhase == GUIDE_APPROACH ? px+60 : 1310;
        float targetY = guidePhase == GUIDE_APPROACH ? py+10 : 230;
        float dx = targetX-npcX, dy = targetY-npcY;
        float length = sqrt(dx*dx+dy*dy);
        float stop = guidePhase == GUIDE_APPROACH ? 35.f : 1.f;

        if(length > stop) {
            float speed = guidePhase == GUIDE_APPROACH ? 90.f : 115.f;
            float step = limit(speed*dt,0,length-stop);
            npcX += dx/length*step;
            npcY += dy/length*step;
        }

        if(distanceTo(npcX,npcY,targetX,targetY) <= stop+.1f) {
            if(guidePhase == GUIDE_APPROACH && !guideGiven) {
                guidePhase = GUIDE_BRIEFING;
                openLevelBriefing();
            }
            else if(guidePhase == GUIDE_RETURN) {
                guidePhase = GUIDE_PATROL;
                npcClock = 0;
                npcX = 1310;
                npcY = 230;
            }
        }
        return;
    }

    npcClock += dt;
    npcX = 1310+90*sin(npcClock*.35f);
    npcY = 230+12*sin(npcClock*.8f);
}

void stopTalking() {
    talking = false;
    clearInput();
}

void openTalk() {
    talking = true;
    dialoguePage = 0;
    pendingDelivery = taskType == 2 && parcel && !taskDone;

    dialogue[0] = string(heroes[npcHero].name)+": Welcome to "+
        worlds[dimension]+"! These paths are never quite safe.";

    dialogue[1] = pendingDelivery ?
        "You brought the supplies! Thank you. This will help us stabilize the rift." :
        objective();

    dialogue[2] = pendingDelivery ?
        "The local task is done. Next, follow the gold marker toward the rift." :
        "Watch for rustling, cracks, or shadows. Choose another lane before it strikes.";
}

void advanceTalk() {
    if(++dialoguePage < 3) return;

    bool delivered = pendingDelivery;
    pendingDelivery = false;
    stopTalking();

    if(delivered) completeLocalTask();
}

void openLevelBriefing() {
    levelBriefing = true;
    clearInput();

    if(taskDone) {
        briefingLines[0] = "LOCAL QUEST: complete. Continue toward the rift.";
    }
    else if(taskType == 0) {
        briefingLines[0] = "QUEST: collect "+to_string(required)+" "+
            supplies[dimension]+
            ". Stand close to each golden item and press E.";
    }
    else if(taskType == 1) {
        briefingLines[0] =
            "QUEST: recover the lost relic in the chest near the eastern ancient arch. Press E beside the chest.";
    }
    else {
        briefingLines[0] =
            string("QUEST: pick up the parcel beside this camp with E, then deliver it to ")+
            heroes[npcHero].name+
            ". Finish the E conversation to deliver it.";
    }

    const char* hazards[] = {
        "DANGER: thorn grass has red tips and rustles before striking. Read the clues and choose a clear lane.",
        "DANGER: cracked ground warns of spikes and collapsing floor. Watch the warning, then dodge or dash.",
        "DANGER: mixed traps include falling rocks. Watch shadows, cracks and red-tipped grass before moving.",
        "DANGER: traps ambush without visible clues. A pink ghost follows you to the portal. Expect tricks around the exit.",
        "DANGER: hidden ambushes and a faster pink ghost. The portal approach has moving orbs and traps behind you.",
        "DANGER: the 100-HP Warden spits fireballs and an 11-fireball fan every four combat seconds. Hidden traps and the ghost remain active."
    };

    briefingLines[1] = hazards[dimension];

    briefingLines[2] = dimension == BOSS_LEVEL ?
        "NEXT: buy a magic focus here (V, 40 coins). Finish the local quest to remove the boss shield. Left-click to cast: close hits 10 damage, far hits 5." :
        "NEXT: complete your local quest, then collect the moving golden key near the eastern rift with E.";

    briefingLines[3] =
        "PORTAL: "+
        string(dimension == BOSS_LEVEL ?
            "defeat the Warden and collect the key first. " : "")+
        "Left-click rune to "+
        to_string(static_cast<int>(targetAngle))+
        " degrees; scroll portal to "+
        to_string(static_cast<int>(targetScale*100+.5f))+
        "%. Press E nearby to cross.";

    briefingLines[4] =
        "CAMP SHOP: B buys faster boots. "+
        string(dimension >= 3 ?
            "G buys a guardian angel (50 coins): +1 heart every "+
            to_string(static_cast<int>(guardianInterval()))+
            " seconds in this level." :
            "Guardian angels unlock in Level 4.");

    briefingLines[5] =
        "CONTROLS: WASD / arrows move. SPACE dashes. E interacts. Follow the gold marker. Your guide visits once per level. Walk near a checkpoint flag to save automatically.";
}

void closeLevelBriefing() {
    levelBriefing = false;

    if(guidePhase == GUIDE_BRIEFING) {
        guideGiven = true;
        guidePhase = GUIDE_RETURN;
    }

    clearInput();
}

// ---------- Quest progression ----------

void resetChallenge() {
    resetGuardian();
    resetPortalGauntlet(true);
    generateObstacles();

    health = 3;
    immunity = 1.5f;
    dashTime = 0;
    dashCooldown = 0;

    checkpointX = 230;
    checkpointY = 160;
    checkpointSet = false;

    wispX = 1580;
    wispY = 240;
    ghostAwake = false;

    bossHP = dimension == BOSS_LEVEL ? BOSS_MAX_HP : 0;
    attackCooldown = 0;
    castTime = 0;
    bossTimer = 1;
    bossHurt = 0;
    barrageTimer = 4;
    spitFlash = 0;
    damageTextTime = 0;
    lastSpellDamage = 0;

    for(int i=0; i<MAX_SHOTS; ++i) shots[i].active = false;
}

void beginQuest() {
    taskType = (completed+completed/LEVEL_COUNT)%3;
    required = 3+(dimension < 2 ? dimension : 2);
    collected = 0;
    taskDone = false;
    parcel = false;
    hasKey = false;

    for(int i=0; i<6; ++i) picked[i] = false;

    relicX = 1460+(completed%3)*170;
    relicY = 170+(completed%2)*135;

    targetAngle = 90+(completed%4)*30;
    targetScale = .9f+(completed%5)*.05f;
    runeAngle = 0;
    portalScale = .7f;

    px = 230;
    py = 160;
    cameraX = 0;
    flash = 1;

    resetChallenge();

    talking = false;
    pendingDelivery = false;
    dialoguePage = 0;

    npcHero = (active+1+(dimension%3))%4;
    npcClock = 0;
    npcX = 200;
    npcY = 285;

    guidePhase = GUIDE_APPROACH;
    guideGiven = false;
    levelBriefing = false;

    clearInput();

    say(string(heroes[npcHero].name)+
        " is coming over to explain this level. Listen to the guide before exploring.");
}

void completeLocalTask() {
    taskDone = true;

    say(bossHP > 0 ?
        "Local task complete! Defeat the Rift Warden near the arch. Buy magic focus at camp (V). Left-click: cast, SPACE: dash." :
        "Local task complete! Find the moving golden key near the rift to the east.");
}

void travel() {
    int reward = 25+required*5;
    int payment = magicDebt < reward ? magicDebt : reward;

    magicDebt -= payment;
    reward -= payment;
    coins += reward;

    ++completed;
    dimension = (dimension+1)%LEVEL_COUNT;

    beginQuest();

    say("Rift crossed! +"+to_string(reward)+
        " coins. Welcome to "+worlds[dimension]+
        ". Your guide is coming.");
}

// ---------- Damage and movement ----------

void startDash() {
    if(dashCooldown > 0) return;

    dashX = facingX;
    dashY = facingY;
    dashTime = .22f;
    dashCooldown = 1.6f;
}

void takeDamage(float sourceX, float sourceY) {
    if(immunity > 0 || dashTime > 0) return;

    --health;
    hurtFlash = 1;
    immunity = 1.4f;

    if(health <= 0) {
        resetPortalGauntlet(false);

        ++deaths;
        int lost = coins < 10 ? coins : 10;
        coins -= lost;

        px = checkpointX;
        py = checkpointY;
        health = 3;
        immunity = 2;

        resetGuardian();
        dashTime = 0;
        clearInput();

        for(int i=0; i<trapCount; ++i) {
            traps[i].phase = 0;
            traps[i].remaining = 0;
        }

        wispX = 1580;
        wispY = 240;
        ghostAwake = false;

        for(int i=0; i<MAX_SHOTS; ++i) shots[i].active = false;

        bossTimer = 1;
        barrageTimer = 4;

        if(dimension == BOSS_LEVEL && bossHP > 0)
            bossHP = BOSS_MAX_HP;

        // Guide state is deliberately preserved after death.
        say("Caught! Returned to checkpoint. Lost "+
            to_string(lost)+" coins; quest progress kept.");
    }
    else {
        float dx = px-sourceX, dy = py-sourceY;
        float length = sqrt(dx*dx+dy*dy);

        if(length < .01f) {
            dx = -1;
            dy = 0;
            length = 1;
        }

        px = limit(px+dx/length*55,35,WORLD-35);
        py = limit(py+dy/length*55,105,350);

        say("Trap hit! "+to_string(health)+
            " hearts left. Dash with SPACE or dodge around hazards.");
    }
}

void updateCheckpoint() {
    // Automatic, once per level. No E key needed.
    if(!checkpointSet && distanceTo(px,py,1200,150) < 65) {
        checkpointSet = true;
        checkpointX = 1200;
        checkpointY = 150;
        health = 3;

        say("Checkpoint saved automatically! Health restored. Falls will return you to this flag.");
    }
}

void updateHazards(float dt) {
    for(int i=0; i<trapCount; ++i) {
        Trap& t = traps[i];

        float triggerRange = dimension >= 3 ? 90.f : 125.f;

        if(t.phase == 0 &&
           distanceTo(px,py,t.x,t.y) < triggerRange) {
            if(dimension >= 3) {
                t.phase = 2;
                t.remaining = t.type == 2 ? .55f : 1.3f;

                say("AMBUSH! Dash away! Remember this spot for your next attempt.");
            }
            else {
                t.phase = 1;
                t.remaining = 1.3f-dimension*.14f;

                say(t.type == 3 ?
                    "Red-tipped grass is twitching! Step away before the thorns rise!" :
                    t.type == 0 ?
                    "The ground is cracking! Spikes incoming!" :
                    t.type == 1 ?
                    "That floor is collapsing! Move off the marked patch!" :
                    "A shadow overhead! Dodge the falling rock!");
            }
        }

        if(t.phase != 0) {
            t.remaining -= dt;

            if(t.remaining <= 0) {
                if(t.phase == 1) {
                    t.phase = 2;
                    t.remaining = t.type == 2 ? .45f : 1.3f;
                }
                else if(t.phase == 2) {
                    t.phase = 3;
                    t.remaining = 2.5f;
                }
                else {
                    t.phase = 0;
                }
            }
        }

        if(t.phase == 2) {
            bool hit = t.type == 2 ?
                distanceTo(px,py,t.x,t.y) < 58 :
                fabs(px-t.x) < 78 && fabs(py-t.y) < 43;

            if(hit) takeDamage(t.x,t.y);
        }
    }

    if(dimension >= 3) {
        if(px > 1400) ghostAwake = true;

        float dx = px-wispX, dy = py-wispY;
        float length = sqrt(dx*dx+dy*dy);

        if(ghostAwake && length > 1) {
            float speed = dimension == BOSS_LEVEL ? 145.f :
                          dimension == 4 ? 155.f : 130.f;

            float step = limit(speed*dt,0,length);

            wispX = limit(wispX+dx/length*step,35,WORLD-35);
            wispY = limit(wispY+dy/length*step,105,350);
        }

        if(ghostAwake && distanceTo(px,py,wispX,wispY) < 35)
            takeDamage(wispX,wispY);
    }
}

// ---------- Left-click magic and boss ----------

void castMagic() {
    if(attackCooldown > 0) return;

    if(!magicOwned) {
        say("Buy a magic focus at the Level 6 camp with V, then left-click to cast.");
        return;
    }

    attackCooldown = .65f;
    castTime = .16f;
    spellEndX = px+facingX*125;
    spellEndY = py+facingY*125;

    float range = distanceTo(px,py,BOSS_X,BOSS_Y);

    if(dimension == BOSS_LEVEL && bossHP > 0) {
        if(!taskDone) {
            say("The Warden is shielded. Complete the local quest first.");
        }
        else {
            int damage = spellDamage(range);

            if(damage > 0) {
                bossHP = bossHP > damage ? bossHP-damage : 0;
                bossHurt = .2f;
                damageTextTime = .7f;
                lastSpellDamage = damage;

                spellEndX = BOSS_X;
                spellEndY = BOSS_Y;

                if(bossHP == 0) {
                    for(int i=0; i<MAX_SHOTS; ++i)
                        shots[i].active = false;

                    say("Warden defeated! Take the key and repair the rift.");
                }
            }
            else {
                say("Out of spell range. Move within 430 units of the Warden.");
            }
        }
    }

    float dx = wispX-px, dy = wispY-py;
    float length = sqrt(dx*dx+dy*dy);

    if(dimension >= 3 && ghostAwake && length <= 105) {
        if(length < 1) {
            dx = facingX;
            dy = facingY;
            length = 1;
        }

        spellEndX = wispX;
        spellEndY = wispY;

        wispX = limit(wispX+dx/length*220,35,WORLD-35);
        wispY = limit(wispY+dy/length*220,105,350);
    }
}

void spitFireballs(int count, float spread, float speed) {
    float mouthX = BOSS_X-18, mouthY = BOSS_Y;
    float aim = atan2(py-mouthY,px-mouthX);

    for(int n=0; n<count; ++n) {
        float offset = count == 1 ? 0 :
            spread*(static_cast<float>(n)/(count-1)-.5f);

        float angle = aim+offset;

        for(int j=0; j<MAX_SHOTS; ++j) {
            if(!shots[j].active) {
                shots[j] = {
                    mouthX, mouthY,
                    speed*cosf(angle), speed*sinf(angle), true
                };
                break;
            }
        }
    }

    spitFlash = .25f;
}

void updateBoss(float dt) {
    attackCooldown = limit(attackCooldown-dt,0,1);
    castTime = limit(castTime-dt,0,1);
    damageTextTime = limit(damageTextTime-dt,0,1);
    bossHurt = limit(bossHurt-dt,0,1);
    spitFlash = limit(spitFlash-dt,0,1);

    if(dimension != BOSS_LEVEL || bossHP <= 0) return;

    if(taskDone && px > 1320) {
        bossTimer -= dt;
        barrageTimer -= dt;

        if(barrageTimer <= 0) {
            barrageTimer += 4;
            spitFireballs(11,2.5f,175);
            bossTimer = .9f;
        }
        else if(bossTimer <= 0) {
            bossTimer = .9f;
            spitFireballs(1,0,210);
        }
    }

    for(int i=0; i<MAX_SHOTS; ++i) {
        if(!shots[i].active) continue;

        Shot& shot = shots[i];
        shot.x += shot.dx*dt;
        shot.y += shot.dy*dt;

        if(shot.x < 1250 || shot.x > WORLD ||
           shot.y < 70 || shot.y > 400) {
            shot.active = false;
        }
        else if(distanceTo(px,py,shot.x,shot.y) < 25) {
            shot.active = false;
            takeDamage(shot.x,shot.y);
        }
    }

    if(distanceTo(px,py,BOSS_X,BOSS_Y) < 47)
        takeDamage(BOSS_X,BOSS_Y);
}

// ---------- Drawing helpers ----------

void color(float r, float g, float b, float a=1) {
    glColor4f(r,g,b,a);
}

void triangle(float x, float y, float a, float b, float c, float d) {
    glBegin(GL_TRIANGLES);
    glVertex2f(x,y);
    glVertex2f(a,b);
    glVertex2f(c,d);
    glEnd();
}

void rect(float x, float y, float width, float height) {
    triangle(x,y,x+width,y,x+width,y+height);
    triangle(x,y,x+width,y+height,x,y+height);
}

void line(float x, float y, float a, float b, float width=2) {
    glLineWidth(width);
    glBegin(GL_LINES);
    glVertex2f(x,y);
    glVertex2f(a,b);
    glEnd();
}

void oval(float x, float y, float rx, float ry) {
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x,y);

    for(int i=0; i<=48; ++i) {
        float angle = 2*PI*i/48;
        glVertex2f(x+rx*cos(angle),y+ry*sin(angle));
    }

    glEnd();
}

void ring(float x, float y, float rx, float ry) {
    glLineWidth(3);
    glBegin(GL_LINE_LOOP);

    for(int i=0; i<64; ++i) {
        float angle = 2*PI*i/64;
        glVertex2f(x+rx*cos(angle),y+ry*sin(angle));
    }

    glEnd();
}

void text(float x, float y, const string& message,
          void* font=GLUT_BITMAP_HELVETICA_12) {
    glRasterPos2f(x,y);

    for(unsigned char c : message)
        glutBitmapCharacter(font,c);
}

// ---------- Portal ambush gauntlet ----------

struct ExitTrap {
    float x, y, timer;
    int kind, phase;
};

ExitTrap exitTraps[11];
bool backTrapSet[3] = {};
float gauntletTime = 0, decoyTime = -1;
bool decoyUsed = false;

void resetPortalGauntlet(bool newLayout) {
    gauntletTime = 0;
    decoyTime = -1;
    decoyUsed = false;

    for(int i=0; i<3; ++i) backTrapSet[i] = false;

    if(newLayout) {
        const float columns[] = {1900,2030,2160,2290};
        const float rows[] = {145,230,315};
        int n = 0;

        for(int column=0; column<4; ++column) {
            int safe = randomInt(0,2);

            for(int lane=0; lane<3; ++lane) {
                if(lane == safe) continue;

                exitTraps[n++] = {
                    columns[column], rows[lane], 0,
                    (column+lane+dimension)%3, 0
                };
            }
        }

        for(int i=8; i<11; ++i)
            exitTraps[i] = {0,0,0,0,0};
    }

    for(int i=0; i<11; ++i) {
        exitTraps[i].timer = 0;
        exitTraps[i].phase = 0;
    }
}

float decoyX() {
    if(decoyTime < 0) return 2320;

    float p = decoyTime < .35f ?
        decoyTime/.35f : 1-(decoyTime-.35f)/1.1f;

    return 2320-180*limit(p,0,1);
}

bool gauntletDamage(float x, float y) {
    int before = deaths;
    takeDamage(x,y);
    return deaths != before;
}

void updatePortalGauntlet(float dt) {
    if(dimension < 3) return;

    gauntletTime += dt;
    if(px < 1780) return;

    const float crosses[] = {1940,2090,2230};

    for(int i=0; i<3; ++i) {
        if(!backTrapSet[i] && px > crosses[i]) {
            backTrapSet[i] = true;
            exitTraps[8+i] = {crosses[i]-75,py,.18f,0,1};

            say("The path behind you just changed! Sidestep before retreating.");
        }
    }

    for(int i=0; i<11; ++i) {
        ExitTrap& t = exitTraps[i];

        if(i < 8 && t.phase == 0 &&
           fabs(px-t.x) < 68 && fabs(py-t.y) < 34) {
            t.phase = 1;
            t.timer = dimension == BOSS_LEVEL ? .08f : .16f;
        }

        if(t.phase != 0) {
            t.timer -= dt;

            if(t.timer <= 0) {
                if(t.phase == 1) {
                    t.phase = 2;
                    t.timer = i >= 8 ? 3.2f :
                        (t.kind == 1 ? 1.9f : 1.15f);
                }
                else if(t.phase == 2) {
                    t.phase = 3;
                    t.timer = 2.2f;
                }
                else {
                    t.phase = 0;
                }
            }
        }

        if(t.phase == 2 &&
           fabs(px-t.x) < 68 && fabs(py-t.y) < 34) {
            if(gauntletDamage(t.x,t.y)) return;
        }
    }

    for(int i=0; i<2; ++i) {
        float x = (i == 0 ? 1960.f : 2220.f)+
                  75*sin(gauntletTime*1.8f+i*PI);
        float y = i == 0 ? 188.f : 275.f;

        if(distanceTo(px,py,x,y) < 29 && gauntletDamage(x,y))
            return;
    }

    if(!decoyUsed && distanceTo(px,py,2320,240) < 105) {
        decoyUsed = true;
        decoyTime = 0;

        say("That exit moved! Dash away from the false portal!");
    }

    if(decoyTime >= 0) {
        decoyTime += dt;

        if(decoyTime < 1.45f &&
           distanceTo(px,py,decoyX(),240) < 49) {
            if(gauntletDamage(decoyX(),240)) return;
        }
    }
}

void drawPortalGauntlet() {
    if(dimension < 3) return;

    for(int i=0; i<11; ++i) {
        const ExitTrap& t = exitTraps[i];
        if(t.phase != 2) continue;

        glPushMatrix();
        glTranslatef(t.x,t.y,0);

        if(t.kind == 1) {
            color(.07f,.08f,.15f);
            oval(0,0,68,34);
            color(.70f,.24f,.55f);
            ring(0,0,64,30);
        }
        else if(t.kind == 2) {
            color(.39f,.33f,.37f);
            oval(0,14,55,40);
            color(.84f,.46f,.34f);
            ring(0,0,64,30);
        }
        else {
            color(.25f,.13f,.21f);
            rect(-68,-28,136,56);
            color(.97f,.29f,.37f);

            for(int tooth=0; tooth<7; ++tooth) {
                float x = -66+tooth*19;
                triangle(x,-22,x+17,-22,x+8,32);
            }
        }

        glPopMatrix();
    }

    for(int i=0; i<2; ++i) {
        float x = (i == 0 ? 1960.f : 2220.f)+
                  75*sin(gauntletTime*1.8f+i*PI);
        float y = i == 0 ? 188.f : 275.f;

        glPushMatrix();
        glTranslatef(x,y+20,0);
        glRotatef(gauntletTime*180,0,0,1);

        color(.91f,.30f,.39f);

        for(int tooth=0; tooth<8; ++tooth) {
            glPushMatrix();
            glRotatef(tooth*45.f,0,0,1);
            triangle(-7,16,7,16,0,32);
            glPopMatrix();
        }

        color(.25f,.17f,.27f);
        oval(0,0,20,20);
        color(1,.80f,.46f);
        oval(-6,1,3,4);
        oval(6,1,3,4);

        glPopMatrix();
    }

    float x = decoyX();

    color(.36f,.87f,.77f);
    oval(x,280,49,84);
    color(.33f,.17f,.46f);
    oval(x,280,41,76);
    color(.09f,.10f,.22f);
    oval(x,280,32,64);
    color(.99f,.82f,.48f);
    text(x-15,378,"EXIT");

    if(decoyUsed) {
        color(1,.25f,.30f);
        oval(x-12,292,5,7);
        oval(x+12,292,5,7);
    }
}

// ---------- Guardian ----------

void buyGuardian() {
    if(dimension < 3) {
        say("Guardian angels are available at Explorer Camp in Levels 4-6.");
        return;
    }

    if(distanceTo(px,py,200,260) > 140) {
        say("Return to Explorer Camp and press G to buy a guardian angel.");
        return;
    }

    if(guardianOwned) {
        say("Your guardian angel is already with you.");
        return;
    }

    if(coins < GUARDIAN_COST) {
        say("A guardian angel costs 50 coins. Complete quests and cross rifts to earn coins.");
        return;
    }

    coins -= GUARDIAN_COST;
    guardianOwned = true;
    resetGuardian();

    say("Guardian angel purchased! It restores one heart every 7s in L4, 5s in L5, and 4s in L6.");
}

void updateGuardian(float dt) {
    if(!guardianOwned) return;

    float interval = guardianInterval();
    guardianGlow = limit(guardianGlow-dt,0,1);

    if(interval <= 0) {
        guardianTimer = 0;
        return;
    }

    float follow = limit(dt*5,0,1);
    angelX += (px-45-angelX)*follow;
    angelY += (py+135-angelY)*follow;

    if(guardianTimer <= 0) guardianTimer = interval;
    guardianTimer -= dt;

    while(guardianTimer <= .0001f) {
        guardianTimer += interval;

        if(health < 3) {
            ++health;
            guardianGlow = 1;
            say("Your guardian angel restored one heart!");
        }
    }
}

string guardianStatus() {
    if(!guardianOwned) return "Angel: G at camp (50 coins)";
    if(guardianInterval() <= 0) return "Angel: resting until Level 4";

    return "Angel: heal in "+
        to_string(static_cast<int>(ceil(guardianTimer)))+"s";
}

void drawGuardian() {
    if(!guardianOwned || guardianInterval() <= 0) return;

    float bob = 4*sin(worldTime*3);
    float flap = 8*sin(worldTime*7);

    glPushMatrix();
    glTranslatef(angelX,angelY+bob,0);

    color(.87f,.91f,1,.30f);
    oval(0,0,36,41);

    color(.95f,.97f,1);
    glPushMatrix();
    glTranslatef(-8,0,0);
    glRotatef(-25-flap,0,0,1);
    oval(-13,0,20,9);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(8,0,0);
    glRotatef(25+flap,0,0,1);
    oval(13,0,20,9);
    glPopMatrix();

    color(.82f,.72f,1);
    triangle(-13,-20,13,-20,0,9);

    color(1,.85f,.69f);
    oval(0,13,10,11);

    color(.18f,.18f,.27f);
    oval(-3,14,1.5f,2);
    oval(3,14,1.5f,2);

    color(1,.88f,.36f);
    ring(0,32,14,5);

    glPopMatrix();

    if(guardianGlow > 0) {
        color(.65f,1,.75f,guardianGlow);
        ring(px,py+40,32+(1-guardianGlow)*30,
             55+(1-guardianGlow)*25);

        text(px-20,py+120,"+1 HP",GLUT_BITMAP_HELVETICA_18);
    }
}

// ---------- Sky and moving background ----------

void cloud(float x, float y, float scale, float light) {
    glPushMatrix();
    glTranslatef(x,y,0);
    glScalef(scale,scale,1);

    color(.50f+.45f*light,.54f+.41f*light,.65f+.32f*light,.9f);
    oval(0,0,48,16);
    oval(-23,12,23,21);
    oval(12,20,30,25);
    oval(42,5,25,17);

    glPopMatrix();
}

void sky() {
    float day = daylightAmount(worldTime);
    float night = 1-day;

    glBegin(GL_TRIANGLES);
    color(.18f+.20f*day,.23f+.46f*day,.40f+.46f*day);
    glVertex2f(0,H);
    glVertex2f(W,H);

    color(.34f+.35f*day,.37f+.44f*day,.51f+.37f*day);
    glVertex2f(0,320);
    glVertex2f(0,320);
    glVertex2f(W,320);

    color(.18f+.20f*day,.23f+.46f*day,.40f+.46f*day);
    glVertex2f(W,H);
    glEnd();

    float orbit = 2*PI*worldTime/60;

    color(1,.88f,.39f);
    oval(600+460*sin(orbit),330+240*cos(orbit),28,28);

    color(.89f,.94f,1);
    oval(600-460*sin(orbit),330-240*cos(orbit),23,23);

    for(int i=0; i<48; ++i) {
        float x = static_cast<float>((i*137+37)%W);
        float y = 430+(i*73)%195;
        float sparkle = .6f+.4f*sin(worldTime*2+i);

        color(.92f,.95f,1,night*night*sparkle);
        line(x-2,y,x+2,y,1);
        line(x,y-2,x,y+2,1);
    }

    float phase = fmod(worldTime,8.f);

    if(phase < 1.2f) {
        float x = 940-phase*420;
        float y = 610-phase*140;

        glLineWidth(3);
        glBegin(GL_LINES);
        color(.8f,.9f,1,0);
        glVertex2f(x+95,y+32);
        color(1,1,1,night*night);
        glVertex2f(x,y);
        glEnd();
    }

    for(int i=0; i<4; ++i) {
        float x = fmod(i*350+worldTime*(12+i*3),1500.f)-140;
        cloud(x,510+(i%3)*38,.75f+i*.13f,day);
    }

    float birdX = fmod(worldTime*85,1400.f)-100;
    float birdY = 480+25*sin(worldTime*.8f);
    float wing = 12*sin(worldTime*11);

    color(.12f,.16f,.23f);
    line(birdX-17,birdY+wing,birdX,birdY,3);
    line(birdX,birdY,birdX+17,birdY+wing,3);
}

float grassSway(float time, float seed) {
    return 5*sin(time*2.1f+seed*.73f);
}

void drawVegetation() {
    for(int i=0; i<105; ++i) {
        float x = 32+i*23;
        float y = 95+(i*67)%230;

        if(x < cameraX-50 || x > cameraX+W+50) continue;

        float sway = grassSway(worldTime,static_cast<float>(i));

        color(.22f,.55f,.26f);
        triangle(x-7,y,x-3,y,x-10+sway,y+20);

        color(.37f,.71f,.32f);
        triangle(x-2,y,x+3,y,x+sway,y+29);

        color(.28f,.62f,.26f);
        triangle(x+3,y,x+8,y,x+12+sway,y+22);
    }

    for(int i=0; i<13; ++i) {
        float x = 370+i*151;
        float y = 205+(i%2)*61;

        if(x < cameraX-70 || x > cameraX+W+70) continue;

        float sway = 7*sin(worldTime*1.9f+i*.9f);

        glPushMatrix();
        glTranslatef(x,y,0);

        color(.20f,.53f,.23f);
        line(0,0,sway,43,4);

        glPushMatrix();
        glTranslatef(0,15,0);
        glRotatef(12*sin(worldTime*2+i),0,0,1);

        color(.34f,.69f,.30f);
        oval(-11,0,15,6);
        oval(12,5,16,6);

        glPopMatrix();

        glTranslatef(sway,47,0);
        glRotatef(6*sin(worldTime*2+i),0,0,1);

        for(int petal=0; petal<6; ++petal) {
            float angle = 2*PI*petal/6;

            if(dimension == 3) color(.86f,.53f,.92f);
            else color(.76f,.53f,.54f);

            oval(13*cos(angle),13*sin(angle),9,9);
        }

        color(.30f,.26f,.28f);
        oval(0,0,11,11);

        color(.10f,.11f,.14f);
        oval(-4,2,1.6f,2);
        oval(4,2,1.6f,2);
        line(-3,-4,3,-4,1);

        glPopMatrix();
    }
}

void scenery() {
    const RGB& ground = grounds[dimension];

    color(ground.r+.13f,ground.g+.10f,ground.b+.10f);

    for(int i=0; i<6; ++i)
        oval(150+i*470,360,300,90+(i%2)*40);

    // VBO rendering.
    color(ground.r,ground.g,ground.b);
    glBindBuffer(GL_ARRAY_BUFFER,terrainVBO);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2,GL_FLOAT,0,reinterpret_cast<void*>(0));
    glDrawArrays(GL_TRIANGLES,0,6);
    glDisableClientState(GL_VERTEX_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER,0);

    color(.68f,.60f,.45f);
    rect(0,130,WORLD,35);

    for(int i=0; i<13; ++i) {
        float x = 60+i*190;
        float y = 310+(i%2)*20;

        if(dimension == 0) {
            color(.25f,.16f,.12f);
            rect(x-7,y,14,75);
            color(.07f,.25f,.20f);
            triangle(x-45,y+35,x+45,y+35,x,y+165);
            color(.10f,.34f,.25f);
            triangle(x-33,y+80,x+33,y+80,x,y+200);
        }
        else if(dimension == 1) {
            color(.31f,.30f,.32f);
            rect(x-5,y,10,155);
            rect(x-36,y+130,72,6);
            color(.16f,.18f,.23f);
            line(x-30,y+130,x+160,y+116,2);
            color(.91f,.35f,.30f);
            oval(x+27,y+126,4,4);
        }
        else if(dimension == 2) {
            color(.30f,.25f,.43f);
            rect(x-6,y,12,80);
            color(.52f,.34f,.66f);
            oval(x,y+105,48,48);
            color(.79f,.63f,1);
            oval(x-16,y+112,8,8);
        }
        else if(dimension == 3) {
            color(.83f,.78f,.58f);
            rect(x-5,y,10,60);
            color(.73f,.37f,.67f);
            oval(x,y+70,42,21);
            color(.99f,.89f,.72f);
            oval(x-14,y+75,6,5);
            oval(x+13,y+80,5,4);
        }
        else if(dimension == 4) {
            color(.38f,.28f,.25f);
            triangle(x-32,y,x+32,y,x+5,y+95);
            color(.99f,.56f,.20f);
            line(x-6,y+12,x+7,y+58,3);
        }
        else {
            color(.38f,.78f,.88f);
            triangle(x-30,y,x+24,y,x,y+150);
            color(.64f,.92f,.97f);
            triangle(x,y,x+24,y,x,y+150);
        }
    }

    color(.49f,.31f,.22f);
    rect(110,260,190,110);

    color(.25f,.19f,.23f);
    triangle(90,370,320,370,205,445);

    color(.88f,.75f,.47f);
    rect(115,370,180,30);

    color(.13f,.13f,.18f);
    text(124,380,dimension == 0 ? "RANGER STATION" : "EXPLORER CAMP");

    color(.22f,.16f,.14f);
    rect(180,260,42,75);

    color(.99f,.85f,.45f);
    rect(130,300,30,35);
    rect(250,300,30,35);

    color(1,.95f,.78f);
    text(120,240,dimension == BOSS_LEVEL ?
        "E: journal   B: boots   V: magic focus (40)" :
        "E: journal / parcel   B: buy boots");

    if(dimension >= 3)
        text(120,218,"G: guardian angel (50 coins)");

    color(.67f,.49f,.31f);
    rect(1260,150,95,12);

    color(.48f,.40f,.34f);
    rect(1700,300,32,140);
    rect(1800,300,32,140);
    rect(1690,430,152,25);

    color(.64f,.67f,.62f);
    oval(840,110,30,14);
    oval(1880,200,35,18);

    drawVegetation();
}

// ---------- Portal, rune, quest objects ----------

void portal() {
    glPushMatrix();
    glTranslatef(PORTAL_X,300,0);
    glScalef(portalScale,portalScale,1);

    color(.27f,.85f,.78f);
    oval(0,0,90,125);
    color(.48f,.30f,.78f);
    oval(0,0,78,113);
    color(.10f,.10f,.27f);
    oval(0,0,66,101);
    color(.75f,.69f,1);
    ring(0,0,54,87);

    for(int i=0; i<12; ++i) {
        float angle = 2*PI*i/12+worldTime*.4f;
        color(.69f,1,.90f);
        oval(84*cos(angle),119*sin(angle),3,3);
    }

    glPopMatrix();

    color(1,.95f,.78f);
    text(PORTAL_X-85,155,"E: repair and cross rift");
}

void rune() {
    color(.30f,.28f,.34f);
    rect(RUNE_X-12,185,24,40);
    oval(RUNE_X,240,40,40);

    color(.97f,.79f,.38f);
    ring(RUNE_X,240,34,34);

    float angle = (90+targetAngle)*PI/180;

    color(.66f,1,.85f);
    oval(RUNE_X+47*cos(angle),240+47*sin(angle),5,5);

    glPushMatrix();
    glTranslatef(RUNE_X,240,0);
    glRotatef(runeAngle,0,0,1);

    for(int i=0; i<8; ++i) {
        glPushMatrix();
        glRotatef(i*45,0,0,1);
        line(0,24,0,30);
        line(0,30,5,25);
        glPopMatrix();
    }

    triangle(0,20,-8,-9,8,-9);
    glPopMatrix();
}

void questObjects() {
    if(!taskDone && taskType == 0) {
        for(int i=0; i<6; ++i) {
            if(picked[i]) continue;

            float y = nodeY[i]+3*sin(worldTime*2+i);

            color(.98f,.81f,.30f);
            oval(nodeX[i],y+12,12,14);

            color(.54f,.94f,.73f);
            triangle(nodeX[i],y+25,nodeX[i]-9,y+35,nodeX[i]+5,y+31);
        }
    }

    if(!taskDone && taskType == 1) {
        color(.95f,.76f,.22f);
        rect(relicX-17,relicY,34,26);

        color(.53f,.28f,.15f);
        rect(relicX-17,relicY+11,34,4);

        color(1,1,.70f);
        oval(relicX,relicY+40+4*sin(worldTime*3),5,7);
    }

    if(!taskDone && taskType == 2 && !parcel) {
        color(.83f,.63f,.34f);
        rect(215,200,24,24);

        color(1,.88f,.60f);
        line(227,200,227,224,3);
    }

    if(!taskDone || hasKey || bossHP > 0) return;

    glPushMatrix();
    glTranslatef(keyX(worldTime),keyY(worldTime),0);

    color(1,.87f,.28f);
    ring(-12,0,11,11);

    // Client-side vertex array for the key shaft and tooth.
    const GLfloat vertices[] = {
        -1,-3, 27,-3, 27,3,
        -1,-3, 27,3, -1,3,
        17,-3, 23,-3, 23,-12,
        17,-3, 23,-12, 17,-12
    };

    glBindBuffer(GL_ARRAY_BUFFER,0);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2,GL_FLOAT,0,vertices);
    glDrawArrays(GL_TRIANGLES,0,12);
    glDisableClientState(GL_VERTEX_ARRAY);

    glPopMatrix();
}

void drawHazards() {
    for(int i=0; i<trapCount; ++i) {
        const Trap& t = traps[i];

        if(dimension >= 3 && t.phase != 2) continue;

        glPushMatrix();
        glTranslatef(t.x,t.y,0);

        if(t.type == 3) {
            float rustle = (t.phase == 1 ? 8.f : 2.f)*
                sin(worldTime*(t.phase == 1 ? 24.f : 2.f)+t.x);

            for(int blade=0; blade<8; ++blade) {
                float x = -65+blade*18;

                color(.17f,.43f,.27f);
                triangle(x-5,-25,x+6,-25,x+rustle,13);

                color(.82f,.23f,.31f);
                triangle(x+rustle,13,x-4+rustle,6,x+4+rustle,6);
            }
        }
        else {
            color(.36f,.35f,.37f);
            rect(-75,-38,150,76);

            color(.65f,.31f,.34f);
            line(-60,-14,-18,7);
            line(-18,7,5,-17);
            line(5,-17,55,18);
        }

        if(t.phase == 1) {
            color(1,.59f,.12f,.5f+.2f*sin(worldTime*25));
            rect(-78,-42,156,84);

            color(1,1,.75f);
            text(-48,50,t.type == 2 ? "LOOK OUT!" : "MOVE / DASH!");

            if(t.type == 2) {
                color(.18f,.16f,.20f,.55f);
                oval(0,0,54,32);
            }
        }

        if(t.phase == 2) {
            if(t.type == 0 || t.type == 3) {
                color(.96f,.30f,.30f);

                for(int j=0; j<6; ++j)
                    triangle(-70+j*24,-30,-48+j*24,-30,-59+j*24,42);
            }
            else if(t.type == 1) {
                color(.12f,.12f,.23f);
                oval(0,0,75,39);

                color(.64f,.25f,.78f);
                ring(0,0,69,33);
            }
            else {
                color(.41f,.36f,.40f);
                oval(0,5,55,43);

                color(.75f,.59f,.45f);
                ring(0,0,61,42);
            }
        }

        glPopMatrix();
    }

    if(dimension >= 3) {
        color(.80f,.30f,.98f,.35f);
        oval(wispX,wispY+22,31,31);

        color(.91f,.55f,1);
        oval(wispX,wispY+22,18,21);

        color(1,.93f,.55f);
        oval(wispX-6,wispY+25,3,4);
        oval(wispX+6,wispY+25,3,4);
    }

    color(.75f,.71f,.48f);
    rect(1196,135,7,95);

    if(checkpointSet) color(.30f,1,.72f);
    else color(1,.81f,.25f);

    triangle(1203,225,1245,209,1203,193);

    color(1,1,.87f);
    text(1120,118,checkpointSet ?
        "CHECKPOINT SAVED" : "Walk near flag: auto-save + heal");
}

void drawBoss() {
    if(dimension != BOSS_LEVEL || bossHP <= 0) return;

    glPushMatrix();
    glTranslatef(BOSS_X,BOSS_Y,0);

    color(.25f,.10f,.32f);
    oval(0,7,55,14);

    color(.60f,.16f,.48f);
    triangle(-22,30,-80,85,-42,-3);
    triangle(22,30,80,85,42,-3);

    if(bossHurt > 0) color(1,.94f,.88f);
    else color(.46f,.18f,.56f);

    oval(0,44,36,51);

    color(.94f,.62f,.23f);
    triangle(-30,76,-9,90,-38,120);
    triangle(30,76,9,90,38,120);

    color(1,.85f,.35f);
    oval(-12,65,6,5);
    oval(12,65,6,5);

    color(.12f,.07f,.10f);
    oval(-18,40,spitFlash > 0 ? 15.f : 9.f,
         spitFlash > 0 ? 12.f : 6.f);

    if(spitFlash > 0) {
        color(1,.55f,.13f);
        oval(-22,40,8,7);
    }

    color(.14f,.08f,.20f);
    rect(-55,136,110,10);

    color(.96f,.30f,.45f);
    rect(-55,136,110*bossHP/static_cast<float>(BOSS_MAX_HP),10);

    color(1,.94f,.81f);
    text(-70,155,string(taskDone ? "WARDEN " : "SHIELDED ")+
        to_string(bossHP)+" / 100 HP");

    if(taskDone && barrageTimer < .65f) {
        color(1,.50f,.20f);
        ring(0,40,49,62);
        text(-45,-25,"FIRE BARRAGE!");
    }

    if(!taskDone) {
        color(.45f,.88f,1,.55f);
        ring(0,45,49,68);
    }

    glPopMatrix();

    for(int i=0; i<MAX_SHOTS; ++i) {
        if(!shots[i].active) continue;

        color(1,.42f,.23f);
        oval(shots[i].x,shots[i].y+40,11,11);

        color(1,.89f,.43f);
        oval(shots[i].x,shots[i].y+40,6,6);
    }
}

// ---------- Characters ----------

void character(float x, float y, int index,
               bool walking, bool showName=true) {
    const Hero& hero = heroes[index];
    float step = walking ? sin(worldTime*12)*5 : 0;

    glPushMatrix();
    glTranslatef(x,y,0);

    if(index == active) {
        if(dashTime > 0) color(.25f,.90f,1,.7f);
        else if(immunity > 0) color(1,.92f,.45f,.65f);
        else color(.85f,.98f,1,.35f);

        ring(0,42,28,52);
    }

    color(.06f,.12f,.13f,.35f);
    oval(0,0,22,7);

    color(.16f,.18f,.25f);
    rect(-14,4+step,9,22);
    rect(5,4-step,9,22);

    color(.96f,.78f,.61f);
    line(-19,47,-26,28-step,6);
    line(19,47,26,28+step,6);

    color(hero.shirt.r,hero.shirt.g,hero.shirt.b);
    rect(-18,25,36,32);

    color(hero.hair.r,hero.hair.g,hero.hair.b);
    oval(0,75,23,25);

    color(.97f,.79f,.62f);
    oval(0,72,18,19);

    color(hero.hair.r,hero.hair.g,hero.hair.b);
    oval(-3,88,19,9);

    color(.12f,.12f,.16f);
    oval(-6,75,2,2);
    oval(6,75,2,2);
    line(-4,64,4,64);

    if(index == active && magicOwned) {
        color(.37f,.76f,1,.35f);
        oval(23,48,13,13);

        color(.69f,.94f,1);
        float glow = 6+2*sin(worldTime*5);
        oval(23,48,glow,glow);
    }

    if(showName) {
        color(1,1,.86f);
        text(-23,108,hero.name);
    }

    glPopMatrix();
}

// ---------- Objectives and marker ----------

string objective() {
    if(guidePhase == GUIDE_APPROACH)
        return string("Wait for ")+heroes[npcHero].name+
            " to come over and explain this level.";

    if(dimension == BOSS_LEVEL && !magicOwned)
        return "Level 6: return to camp and buy your magic focus with V (40 coins; credit available).";

    if(!taskDone) {
        if(taskType == 0)
            return "Collect "+to_string(required)+" "+
                supplies[dimension]+" with E ("+
                to_string(collected)+"/"+to_string(required)+").";

        if(taskType == 1)
            return "Find the lost relic near the ancient arch. Press E beside its chest.";

        if(!parcel)
            return "Pick up the supply parcel at camp with E.";

        return string("Deliver the parcel to ")+
            heroes[npcHero].name+" at the eastern outpost.";
    }

    if(bossHP > 0)
        return "Defeat the Warden: Left-click casts magic. Close: 10 damage. Far: 5 damage. SPACE: dodge fireballs.";

    if(!hasKey)
        return "Local quest complete! Collect the moving golden key near the eastern rift (E).";

    return "Set rune to "+
        to_string(static_cast<int>(targetAngle))+
        " deg and portal to "+
        to_string(static_cast<int>(targetScale*100+.5f))+
        "%, then press E at rift.";
}

void target(float& x, float& y) {
    if(dimension == BOSS_LEVEL && !magicOwned) {
        x = 200;
        y = 260;
        return;
    }

    x = PORTAL_X;
    y = 250;

    if(taskDone) {
        if(bossHP > 0) {
            x = BOSS_X;
            y = BOSS_Y;
            return;
        }

        if(!hasKey) {
            x = keyX(worldTime);
            y = keyY(worldTime);
        }
        return;
    }

    if(taskType == 1) {
        x = relicX;
        y = relicY;
    }

    if(taskType == 2) {
        x = parcel ? npcX : 225;
        y = parcel ? npcY : 210;
    }

    if(taskType == 0) {
        float best = 1e9f;

        for(int i=0; i<6; ++i) {
            if(picked[i]) continue;

            float range = distanceTo(px,py,nodeX[i],nodeY[i]);

            if(range < best) {
                best = range;
                x = nodeX[i];
                y = nodeY[i];
            }
        }
    }
}

void drawAtmosphere() {
    for(int i=0; i<32; ++i) {
        float x = fmod(i*83+worldTime*(7+i%4),WORLD);
        float y = 120+fmod(i*61+worldTime*9,380.f);

        color(.86f,.63f,.58f,.20f+.20f*sin(worldTime+i));
        oval(x,y,1.8f,1.8f);
    }

    for(int i=0; i<5; ++i) {
        float x = fmod(i*560+worldTime*12,WORLD+500)-250;

        color(.60f,.68f,.75f,.055f);
        oval(x,115+(i%2)*135,250,30);
    }

    for(int i=0; i<18; ++i) {
        float x = 60+i*140;

        color(.18f,.16f,.23f,.65f);
        line(x,90,x+35,105,3);
        line(x+35,105,x+50,95,3);
        line(x+35,105,x+45,122,2);
    }
}

// ---------- Character selection menu ----------

bool menuContains(float x, float y, float left, float bottom,
                  float width, float height) {
    return x >= left && x <= left+width &&
           y >= bottom && y <= bottom+height;
}

int menuCharacterAt(float x, float y) {
    for(int i=0; i<4; ++i) {
        if(menuContains(x,y,80+i*275,205,215,310))
            return i;
    }

    return -1;
}

void startAdventure() {
    if(!choosingCharacter) return;

    active = menuSelection;
    beginQuest();
    choosingCharacter = false;
}

void menuClick(float x, float y) {
    int index = menuCharacterAt(x,y);

    if(index >= 0) {
        menuSelection = index;
        active = index;
    }
    else if(menuContains(x,y,450,85,300,64)) {
        startAdventure();
    }
}

void centeredMenuText(float center, float y,
                      const string& message, void* font) {
    int pixels = 0;

    for(unsigned char letter : message)
        pixels += glutBitmapWidth(font,letter);

    float width = pixels*static_cast<float>(W)/vw;
    text(center-width/2,y,message,font);
}

void drawCharacterMenu() {
    color(.035f,.065f,.12f);
    rect(0,0,W,H);

    for(int i=0; i<36; ++i) {
        float x = fmod(i*137+menuTime*(5+i%3),static_cast<float>(W));
        float y = 30+(i*79)%650;

        color(.49f,.77f,.85f,.25f+.15f*sin(menuTime+i));
        oval(x,y,2,2);
    }

    color(.67f,.97f,.84f);
    centeredMenuText(600,650,"BRIARWOOD: THE OTHER SIDE",
                     GLUT_BITMAP_TIMES_ROMAN_24);

    color(1,.93f,.80f);
    centeredMenuText(600,603,"CHOOSE YOUR CHARACTER",
                     GLUT_BITMAP_HELVETICA_18);

    centeredMenuText(600,570,
        "Press 1-4 or click a character. Press ENTER or click START when ready.",
        GLUT_BITMAP_HELVETICA_12);

    for(int i=0; i<4; ++i) {
        float x = 80+i*275;
        bool selected = i == menuSelection;

        color(selected ? .13f : .07f,
              selected ? .24f : .12f,
              selected ? .28f : .19f);

        rect(x,205,215,310);

        if(selected) color(1,.83f,.37f);
        else color(.25f,.36f,.44f);

        line(x,205,x+215,205,3);
        line(x+215,205,x+215,515,3);
        line(x+215,515,x,515,3);
        line(x,515,x,205,3);

        color(1,.95f,.84f);
        centeredMenuText(x+107.5f,478,heroes[i].name,
                         GLUT_BITMAP_HELVETICA_18);

        glPushMatrix();
        glTranslatef(x+107.5f,285,0);
        glScalef(1.4f,1.4f,1);
        character(0,0,i,false,false);
        glPopMatrix();

        color(.75f,.90f,.91f);
        centeredMenuText(x+107.5f,240,
            selected ? "SELECTED" : "Press "+to_string(i+1),
            GLUT_BITMAP_HELVETICA_12);
    }

    color(1,.91f,.70f);
    centeredMenuText(600,178,
        string("Selected: ")+heroes[menuSelection].name,
        GLUT_BITMAP_HELVETICA_18);

    color(.23f,.64f,.49f);
    rect(450,85,300,64);

    color(1,1,.88f);
    centeredMenuText(600,111,"START ADVENTURE",
                     GLUT_BITMAP_HELVETICA_18);

    color(.73f,.82f,.86f);
    centeredMenuText(600,35,
        "Explore. Read the traps. Repair the rifts.     ESC: exit",
        GLUT_BITMAP_HELVETICA_12);
}

// ---------- Guide panel ----------

void wrappedBriefingText(float x, float y, const string& message) {
    string lineText;
    size_t position = 0;
    float maximumPixels = 980*static_cast<float>(vw)/W;

    while(position < message.size()) {
        size_t end = message.find(' ',position);
        if(end == string::npos) end = message.size();

        string word = message.substr(position,end-position);
        string trial = lineText.empty() ? word : lineText+" "+word;
        int pixels = 0;

        for(unsigned char letter : trial)
            pixels += glutBitmapWidth(GLUT_BITMAP_HELVETICA_12,letter);

        if(pixels > maximumPixels && !lineText.empty()) {
            text(x,y,lineText);
            y -= 17;
            lineText = word;
        }
        else {
            lineText = trial;
        }

        position = end+1;
    }

    if(!lineText.empty()) text(x,y,lineText);
}

void drawLevelBriefing() {
    color(.01f,.025f,.055f,.65f);
    rect(0,0,W,H);

    color(.055f,.095f,.15f,.99f);
    rect(80,180,1040,390);

    color(.65f,.96f,.82f);
    centeredMenuText(600,540,
        string(heroes[npcHero].name)+"'S GUIDE - LEVEL "+
        to_string(dimension+1)+": "+worlds[dimension],
        GLUT_BITMAP_HELVETICA_18);

    for(int i=0; i<6; ++i) {
        if(i == 1) color(1,.72f,.57f);
        else color(.94f,.95f,.86f);

        wrappedBriefingText(110,500-i*43,briefingLines[i]);
    }

    color(.25f,.62f,.49f);
    rect(400,192,400,40);

    color(1,1,.88f);
    centeredMenuText(600,206,"E / ENTER: START EXPLORING",
                     GLUT_BITMAP_HELVETICA_18);
}

// ---------- Main rendering ----------

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if(choosingCharacter) {
        drawCharacterMenu();
        glutSwapBuffers();
        return;
    }

    sky();

    glPushMatrix();
    glTranslatef(-cameraX,0,0);

    scenery();

    // Readable night lighting.
    color(.10f,.13f,.27f,.16f*(1-daylightAmount(worldTime)));
    rect(0,80,WORLD,285);

    drawAtmosphere();
    drawHazards();
    drawPortalGauntlet();
    drawBoss();
    portal();
    rune();
    questObjects();

    bool moving = held['w'] || held['a'] ||
                  held['s'] || held['d'] ||
                  arrows[0] || arrows[1] || arrows[2] || arrows[3];

    if(levelBriefing || guidePhase == GUIDE_APPROACH)
        moving = false;

    if(py < npcY)
        character(npcX,npcY,npcHero,!talking && !levelBriefing);

    character(px,py,active,moving);

    if(py >= npcY)
        character(npcX,npcY,npcHero,!talking && !levelBriefing);

    drawGuardian();

    color(1,.95f,.80f);
    string npcLabel;

    if(guidePhase == GUIDE_APPROACH) npcLabel = "Coming to guide you...";
    else if(guidePhase == GUIDE_BRIEFING) npcLabel = "Your guide";
    else if(taskType == 2 && parcel && !taskDone)
        npcLabel = "E: deliver parcel";
    else npcLabel = "Local explorer";

    text(npcX-52,npcY-22,npcLabel);

    if(castTime > 0) {
        color(.27f,.70f,1,.5f);
        line(px+20,py+48,spellEndX,spellEndY+45,9);

        color(.78f,.98f,1);
        line(px+20,py+48,spellEndX,spellEndY+45,3);
        ring(spellEndX,spellEndY+45,
             12+castTime*45,12+castTime*45);
    }

    if(damageTextTime > 0) {
        color(1,.89f,.42f);
        text(BOSS_X-15,BOSS_Y+110+(1-damageTextTime)*25,
             "-"+to_string(lastSpellDamage),
             GLUT_BITMAP_HELVETICA_18);
    }

    glPopMatrix();

    float targetX, targetY;
    target(targetX,targetY);

    float screenTarget = targetX-cameraX;
    float marker = limit(screenTarget,100,W-100);

    color(.04f,.08f,.14f,.90f);
    rect(marker-85,490,170,45);

    color(1,.88f,.31f);
    string direction = "GOAL BELOW";

    if(screenTarget < 100) {
        triangle(marker-72,512,marker-58,520,marker-58,504);
        direction = "< GO LEFT";
    }
    else if(screenTarget > W-100) {
        triangle(marker+72,512,marker+58,520,marker+58,504);
        direction = "GO RIGHT >";
    }
    else {
        triangle(marker,486,marker-8,498,marker+8,498);
    }

    text(marker-45,516,direction);
    text(marker-62,501,
        to_string(static_cast<int>(distanceTo(px,py,targetX,targetY)))+
        " steps to goal");

    color(.04f,.08f,.14f,.96f);
    rect(0,630,W,90);
    rect(0,0,W,80);

    color(.67f,.97f,.84f);
    text(20,686,"BRIARWOOD: THE OTHER SIDE",
         GLUT_BITMAP_TIMES_ROMAN_24);

    color(1,.94f,.81f);
    text(520,687,
        "L"+to_string(dimension+1)+"/6 "+difficulties[dimension]+
        " | "+worlds[dimension]+
        " | Rifts: "+to_string(completed)+
        " | Coins: "+to_string(coins)+
        " | HP: "+to_string(health)+"/3");

    text(20,652,objective(),GLUT_BITMAP_HELVETICA_18);

    text(20,57,
        "WASD / arrows: walk   SPACE: dash   Left-click: magic   E: interact   Auto checkpoints   1-4: hero   B: boots   V: focus   G: angel");

    text(20,92,
        "DASH: "+string(dashCooldown <= 0 ? "READY" : "recharging")+
        "   Boots "+to_string(boots)+"/3"+
        "   Falls "+to_string(deaths)+
        "   Checkpoint: "+(checkpointSet ? "ACTIVE" : "not reached")+
        "   Magic focus: "+(magicOwned ? "EQUIPPED" : "not owned")+
        "   Debt: "+to_string(magicDebt)+
        "   "+guardianStatus());

    text(20,35,
        "Click rune: rotate   Scroll: resize portal   Rune "+
        to_string(static_cast<int>(runeAngle))+
        " deg / target "+to_string(static_cast<int>(targetAngle))+
        "   Portal "+
        to_string(static_cast<int>(portalScale*100+.5f))+
        "% / target "+
        to_string(static_cast<int>(targetScale*100+.5f))+"%");

    color(.22f,.28f,.34f);
    rect(20,14,1160,6);

    color(.66f,.96f,.84f);
    rect(20,14,1160*px/WORLD,6);

    if(worldTime < noticeUntil) {
        color(.04f,.08f,.14f,.90f);
        rect(100,580,1000,34);

        color(1,.94f,.81f);
        text(115,592,notice);
    }

    if(talking) {
        color(.03f,.06f,.12f,.97f);
        rect(60,365,1080,115);

        color(.65f,.98f,.82f);
        text(80,453,
            string(heroes[npcHero].name)+"  ["+
            to_string(dialoguePage+1)+"/3]",
            GLUT_BITMAP_HELVETICA_18);

        color(1,.96f,.85f);
        text(80,422,dialogue[dialoguePage]);
        text(80,392,
            "E: next / finish conversation     Q: close     Movement and hazards pause while talking.");
    }

    if(hurtFlash > 0) {
        color(1,.12f,.16f,hurtFlash*.25f);
        rect(0,0,W,H);
    }

    if(flash > 0) {
        color(.70f,.95f,1,flash);
        rect(0,0,W,H);
    }

    if(levelBriefing) drawLevelBriefing();

    glutSwapBuffers();
}

void reshape(int width, int height) {
    if(width < 1) width = 1;
    if(height < 1) height = 1;

    windowH = height;
    vw = width;
    vh = static_cast<int>(width*static_cast<float>(H)/W);

    if(vh > height) {
        vh = height;
        vw = static_cast<int>(height*static_cast<float>(W)/H);
    }

    vx = (width-vw)/2;
    vy = (height-vh)/2;

    glViewport(vx,vy,vw,vh);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0,W,0,H,-1,1);
    glMatrixMode(GL_MODELVIEW);
}

// ---------- Interaction and shop ----------

void interact() {
    if(guidePhase == GUIDE_APPROACH) return;

    if(levelBriefing) {
        closeLevelBriefing();
        return;
    }

    if(talking) {
        advanceTalk();
        return;
    }

    if(taskType == 2 && parcel && !taskDone &&
       distanceTo(px,py,npcX,npcY) < 90) {
        openTalk();
        return;
    }

    if(!taskDone) {
        if(taskType == 0) {
            for(int i=0; i<6; ++i) {
                if(!picked[i] &&
                   distanceTo(px,py,nodeX[i],nodeY[i]) < 65) {
                    picked[i] = true;
                    ++collected;

                    if(collected >= required) completeLocalTask();
                    else say("Collected "+to_string(collected)+
                        " / "+to_string(required)+" "+
                        supplies[dimension]+".");

                    return;
                }
            }
        }

        if(taskType == 1 &&
           distanceTo(px,py,relicX,relicY) < 70) {
            completeLocalTask();
            return;
        }

        if(taskType == 2 && !parcel &&
           distanceTo(px,py,225,210) < 90) {
            parcel = true;
            say("Parcel collected. Deliver it to the contact at the eastern outpost.");
            return;
        }
    }

    if(taskDone && bossHP == 0 && !hasKey &&
       distanceTo(px,py,keyX(worldTime),keyY(worldTime)) < 80) {
        hasKey = true;
        say("Key collected. Match the rune and portal targets shown below, then press E at the rift.");
        return;
    }

    if(distanceTo(px,py,PORTAL_X,260) < 115) {
        if(!taskDone)
            say("The rift is sealed. Complete the local quest first.");
        else if(bossHP > 0)
            say("Defeat the Rift Warden first. Buy a magic focus (V at camp), cast with left-click (near: 10 damage, far: 5), and dash with SPACE.");
        else if(!hasKey)
            say("Collect the moving golden key nearby first.");
        else if(!tuned())
            say("Match BOTH targets: click the rune wheel and scroll to resize the portal.");
        else
            travel();

        return;
    }

    if(distanceTo(px,py,200,260) < 130 ||
       distanceTo(px,py,npcX,npcY) < 90) {
        say(objective());
    }
    else {
        say("Follow the gold objective marker. Stand near an object and press E.");
    }
}

void buyBoots() {
    int cost = 30+boots*20;

    if(distanceTo(px,py,200,260) > 140) {
        say("Boot upgrades are sold at the camp near your arrival point.");
        return;
    }

    if(boots >= 3) {
        say("Your explorer boots are fully upgraded.");
        return;
    }

    if(coins < cost) {
        say("Boot upgrade costs "+to_string(cost)+
            " coins. Repair rifts to earn coins.");
        return;
    }

    coins -= cost;
    ++boots;
    say("Boots upgraded! You now travel faster in every dimension.");
}

void buyMagic() {
    if(dimension != BOSS_LEVEL) {
        say("Magic focuses are sold at the Level 6 camp.");
        return;
    }

    if(distanceTo(px,py,200,260) > 140) {
        say("Return to the arrival camp and press V to buy your magic focus.");
        return;
    }

    if(magicOwned) {
        say("You already own a magic focus. Left-click: cast, SPACE: dash.");
        return;
    }

    int paid = coins < MAGIC_COST ? coins : MAGIC_COST;
    coins -= paid;
    magicDebt = MAGIC_COST-paid;
    magicOwned = true;

    say(magicDebt > 0 ?
        "Magic focus bought! The camp will take "+
        to_string(magicDebt)+
        " coins from your next rift reward. Left-click: cast." :
        "Magic focus bought for 40 coins! Left-click to cast. Close spells deal 10 damage; distant spells deal 5.");
}

// ---------- Keyboard and mouse ----------

unsigned char lowerKey(unsigned char key) {
    return key >= 'A' && key <= 'Z' ? key+('a'-'A') : key;
}

void keyDown(unsigned char raw, int, int) {
    unsigned char key = lowerKey(raw);

    if(key == 27) {
        glDeleteBuffers(1,&terrainVBO);
        exit(0);
    }

    bool first = !held[key];
    held[key] = true;
    if(!first) return;

    if(choosingCharacter) {
        if(key >= '1' && key <= '4') {
            menuSelection = key-'1';
            active = menuSelection;
        }

        if(key == 13 || key == ' ') startAdventure();

        glutPostRedisplay();
        return;
    }

    if(guidePhase == GUIDE_APPROACH) {
        glutPostRedisplay();
        return;
    }

    if(levelBriefing) {
        if(key == 'e' || key == 13) closeLevelBriefing();
        glutPostRedisplay();
        return;
    }

    if(talking) {
        if(key == 'e') advanceTalk();

        if(key == 'q') {
            pendingDelivery = false;
            stopTalking();
        }
        return;
    }

    if(key >= '1' && key <= '4') {
        active = key-'1';

        if(npcHero == active)
            npcHero = (active+1)%4;

        say(string("Exploring as ")+heroes[active].name+".");
    }

    if(key == 'e') interact();
    if(key == 'b') buyBoots();
    if(key == ' ') startDash();
    if(key == 'v') buyMagic();
    if(key == 'g') buyGuardian();
}

void keyUp(unsigned char key, int, int) {
    held[lowerKey(key)] = false;
}

int arrowIndex(int key) {
    if(key == GLUT_KEY_LEFT) return 0;
    if(key == GLUT_KEY_RIGHT) return 1;
    if(key == GLUT_KEY_UP) return 2;
    if(key == GLUT_KEY_DOWN) return 3;
    return -1;
}

void specialDown(int key, int, int) {
    int index = arrowIndex(key);
    if(index >= 0) arrows[index] = true;
}

void specialUp(int key, int, int) {
    int index = arrowIndex(key);
    if(index >= 0) arrows[index] = false;
}

void mouse(int button, int state, int x, int y) {
    if(state != GLUT_DOWN) return;

    float screenX = (x-vx)*static_cast<float>(W)/vw;
    float screenY = (windowH-y-vy)*static_cast<float>(H)/vh;

    if(choosingCharacter) {
        if(button == GLUT_LEFT_BUTTON)
            menuClick(screenX,screenY);

        glutPostRedisplay();
        return;
    }

    if(guidePhase == GUIDE_APPROACH || levelBriefing || talking)
        return;

    if(button == 3 || button == 4) {
        float change = button == 3 ? .05f : -.05f;

        portalScale = limit(
            round((portalScale+change)*100)/100,
            .65f,1.4f
        );
    }

    if(button == GLUT_LEFT_BUTTON) {
        float worldX = screenX+cameraX;

        // Rune clicks rotate the rune. Other left-clicks cast magic.
        if(distanceTo(worldX,screenY,RUNE_X,240) <= 40) {
            runeAngle = fmod(runeAngle+30,360.f);
        }
        else {
            castMagic();
        }
    }

    glutPostRedisplay();
}

// ---------- Timed game update ----------

void updateStep(float dt) {
    if(choosingCharacter) {
        menuTime += dt;
        return;
    }

    if(guidePhase == GUIDE_APPROACH) {
        flash = limit(flash-dt*1.8f,0,1);
        updateNPC(dt);
        return;
    }

    if(levelBriefing) {
        flash = limit(flash-dt*1.8f,0,1);
        return;
    }

    worldTime += dt;
    flash = limit(flash-dt*1.8f,0,1);

    // Talking pauses movement, enemies, traps and healing timers.
    if(talking) return;

    updateNPC(dt);

    float dx =
        (held['d'] || arrows[1] ? 1.f : 0.f) -
        (held['a'] || arrows[0] ? 1.f : 0.f);

    float dy =
        (held['w'] || arrows[2] ? 1.f : 0.f) -
        (held['s'] || arrows[3] ? 1.f : 0.f);

    float length = sqrt(dx*dx+dy*dy);

    if(length > 0) {
        facingX = dx/length;
        facingY = dy/length;
    }

    if(dashTime > 0) {
        dx = dashX;
        dy = dashY;
        length = 1;
    }

    if(length > 0) {
        float speed = (210+boots*35)*(dashTime > 0 ? 3.2f : 1.f);

        px = limit(px+dx/length*speed*dt,35,WORLD-35);
        py = limit(py+dy/length*speed*dt,105,350);
    }

    updateCheckpoint();
    updateHazards(dt);
    updateBoss(dt);
    updatePortalGauntlet(dt);
    updateGuardian(dt);

    immunity = limit(immunity-dt,0,3);
    dashTime = limit(dashTime-dt,0,1);
    dashCooldown = limit(dashCooldown-dt,0,3);
    hurtFlash = limit(hurtFlash-dt*3,0,1);

    cameraX = limit(px-W*.45f,0,WORLD-W);
}

void update(float dt) {
    // Small steps prevent fast dashes from skipping collision checks.
    while(dt > 0) {
        float step = dt > .016f ? .016f : dt;
        updateStep(step);
        dt -= step;
    }
}

void timer(int) {
    int now = glutGet(GLUT_ELAPSED_TIME);
    float dt = limit((now-lastTick)/1000.f,0,.1f);
    lastTick = now;

    update(dt);
    glutPostRedisplay();
    glutTimerFunc(16,timer,0);
}

// ---------- Entry point ----------

int main(int argc, char** argv) {
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(W,H);
    glutCreateWindow("Briarwood: The Other Side");

#ifndef __APPLE__
    if(glewInit() != GLEW_OK || !GLEW_VERSION_1_5) {
        cerr << "OpenGL 1.5 with VBO support is required.\n";
        return 1;
    }
#endif

    glClearColor(.03f,.05f,.10f,1);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    glGenBuffers(1,&terrainVBO);
    glBindBuffer(GL_ARRAY_BUFFER,terrainVBO);
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(terrain),
        terrain,
        GL_STATIC_DRAW
    );
    glBindBuffer(GL_ARRAY_BUFFER,0);

    beginQuest();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);

    glutKeyboardFunc(keyDown);
    glutKeyboardUpFunc(keyUp);
    glutIgnoreKeyRepeat(1);

    glutSpecialFunc(specialDown);
    glutSpecialUpFunc(specialUp);

    lastTick = glutGet(GLUT_ELAPSED_TIME);
    glutTimerFunc(16,timer,0);

    glutFullScreen();
    glutMainLoop();
    return 0;
}
