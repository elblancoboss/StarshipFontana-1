#include "SFApp.h"

SFApp::SFApp(std::shared_ptr<SFWindow> window) : fire(0),Points(0), is_running(true),is_paused(false), sf_window(window) {
  int canvas_w, canvas_h;
  SDL_GetRendererOutputSize(sf_window->getRenderer(), &canvas_w, &canvas_h);

  app_box = make_shared<SFBoundingBox>(Vector2(canvas_w, canvas_h), canvas_w, canvas_h);
  player  = make_shared<SFAsset>(SFASSET_PLAYER, sf_window);
  auto player_pos = Point2(canvas_w/2, 88.0f);
  player->SetPosition(player_pos);

gameover = make_shared<SFAsset>(SFASSET_GAMEOVER, sf_window);
 auto go_pos = Point2(0,0);
 gameover->SetPosition(go_pos);

// spawn of aliens
const int number_of_aliens = 30;
for(int i=0; i<number_of_aliens; i++) {
auto alien = make_shared<SFAsset>(SFASSET_ALIEN, sf_window);
auto pos = Point2 (rand() % (40 + 600), rand() %(600 + 3000));
alien->SetPosition(pos);
aliens.push_back(alien);
}

//spawn of Alien Missiles
const int number_of_alienmissiles = 30;
for(int i=0; i<number_of_alienmissiles; i++) {
auto alienmissile = make_shared<SFAsset>(SFASSET_ALIENMISSILE, sf_window);
auto alienmissile_pos = Point2 (rand() % (40 + 600), rand() %(600 + 3000));
alienmissile->SetPosition(alienmissile_pos);
alienmissiles.push_back(alienmissile);
}


 //spawn of coins
 const int number_of_coins = 1;
 for(int i=0; i<number_of_coins; i++) {
 auto coin = make_shared<SFAsset>(SFASSET_COIN, sf_window);
 auto pos = Point2 (rand() % (40 + 600), rand() %(600 + 3000));
 coin->SetPosition(pos);
 coins.push_back(coin);
}

//Spawn healthpack
 const int number_of_healthpacks = 1;
 for(int i=0; i<number_of_healthpacks; i++) {
 auto healthpack = make_shared<SFAsset>(SFASSET_HEALTHPACK, sf_window);
 auto hp_pos = Point2 (320,5000);
 healthpack->SetPosition(hp_pos);
healthpacks.push_back(healthpack);
 }

//spawn of stars
 const int number_of_stars = 25;
 for(int i=0; i<number_of_stars; i++) {
 auto star = make_shared<SFAsset>(SFASSET_STAR, sf_window);
 auto pos = Point2 (rand() %(40 + 600), rand() %(600+800));
 star->SetPosition(pos);
 stars.push_back(star);
 }

 //spawn of walls
 for(int i=2; i<5; i++) {
 auto wall = make_shared<SFAsset>(SFASSET_WALL, sf_window);
 auto pos = Point2(100 * i, 400.0f);
 wall->SetPosition(pos);
 walls.push_back(wall);}
}


SFApp::~SFApp() {
}

/**
 * Handle all events that come from SDL.
 * These are timer or keyboard events.
 */
void SFApp::OnEvent(SFEvent& event) {
 SFEVENT the_event = event.GetCode();

 switch(the_event) {
 case SFEVENT_UPDATE: {
 if(!is_paused){
 OnUpdateWorld();
 }
 OnRender();
 break;
 }
 case SFEVENT_QUIT: {
 is_running = false;
 restart = false;
 break;
 }
 case SFEVENT_PAUSE:{
 Pause();
 break;
 }
 case SFEVENT_RESTART:{

 break;
 }
 case SFEVENT_FIRE: {
 if(!is_paused){
 if(fire < fireN){
 fire++;
 FireProjectile();
 }
 }
 //Break out of statement.
 break;
 }
 break;
 }

}

int SFApp::OnExecute() {
  // Execute the app
  SDL_Event event;
  while (SDL_WaitEvent(&event) && is_running) {
    // if this is an update event, then handle it in SFApp,
    // otherwise punt it to the SFEventDispacher.
    SFEvent sfevent((const SDL_Event) event);
    OnEvent(sfevent);
  }
}

void SFApp::OnUpdateWorld() {

const Uint8 *keyboardState = SDL_GetKeyboardState(NULL);
 if(keyboardState[SDL_SCANCODE_DOWN]) {
 player->GoSouth();
 for( auto w : walls){ 
 if(player->CollidesWith(w)){
player->GoNorth();}
 }
 }
 if(keyboardState[SDL_SCANCODE_UP]) {
 player->GoNorth();
  for( auto w : walls){ 
  if(player->CollidesWith(w)){
player->GoSouth();}
 }
 }
 if(keyboardState[SDL_SCANCODE_LEFT]) {
 player->GoWest();
  for( auto w : walls){ 
  if(player->CollidesWith(w)){
player->GoEast();}
 }
 }
 if(keyboardState[SDL_SCANCODE_RIGHT]) {
 player->GoEast();
 for( auto w : walls){ 
 if(player->CollidesWith(w)){
player->GoWest();}
 }
 }


int w, h;
SDL_GetRendererOutputSize(sf_window->getRenderer(), &w, &h);

//check player health
if(PlayerHP <= 0){
player->HandleCollision();
GameOver();
}

// star movement
 for(auto s: stars){
 s->CoinN();
 }

// Update projectile positions
for(auto p: projectiles) {
    p->GoNorth();

// Remove projectile when it hits the top of the screen
 auto p_pos = p->GetPosition();
 if(p_pos.getY() > h) {
 p->HandleCollision();
 }
 }

// healthpack movement
 for(auto hp : healthpacks){
 hp->CoinN();

// when player collects health you gaon +25 health
 if(player->CollidesWith(hp)) {
 PlayerHP = PlayerHP + 25;
 hp->HandleCollision();
 cout << "Health Boost!" << endl;
 cout << "HP"<< PlayerHP << endl;
 }
 }

// coin movement and collision
  for(auto c: coins) {
    c->CoinN();

// when player collects a coin gives you +250 points boost!
 if(player->CollidesWith(c)) {
 Points = Points + 250;
 HealthPackSeed = HealthPackSeed + 250;
 c->HandleCollision();
 cout << "Score Boost!" << endl;
 cout << "Score:" << Points << endl;
 }
 }

// Update alien positions
 for(auto a : aliens) {
 a->AlienN();

// Check player collision with alien, a collition with an alien will take -10 health
 if(player->CollidesWith(a)) {
 PlayerHP = PlayerHP - 10;
 a->HandleCollision();
cout << "Hit by alien!" << endl;
 cout << "HP"<< PlayerHP << endl;
 }
 }

// Update enemy positions
 for(auto d : alienmissiles) {
 d->AlienMissileN();

// Check player collision with alien missile, a hit will cost -10 health.
 if(player->CollidesWith(d)) {
 PlayerHP = PlayerHP - 10;
 d->HandleCollision();
 cout << "Hit by Alien Missile!" << endl;
 cout << "HP"<< PlayerHP << endl;
 }
}

// Detect projectile collision with alien, killing an alien will give you +10 points.
 for(auto p : projectiles) {
 for(auto a : aliens) {
 if(p->CollidesWith(a)) {
 HealthPackSeed = HealthPackSeed + 10;
 Points = Points + 10;
 p->HandleCollision();
 a->HandleCollision();
 cout << "Enemy Killed!" << endl;
 cout << "Score:" << Points << endl;
 }
 }
 }
  


// Detect projectile collision with alienmissile, destroying an alien missile will give you +5 points.
 for(auto p : projectiles) {
 for(auto d : alienmissiles) {
 if(p->CollidesWith(d)) {
 HealthPackSeed = HealthPackSeed + 5;
 Points = Points + 5;
 p->HandleCollision();
 d->HandleCollision();
 cout << "Alien Missile destroyed!" << endl;
cout << "Score:" << Points << endl;
}
}
}

// Remove all dead projectiles
 list<shared_ptr<SFAsset>> projTemp;
 for(auto p : projectiles) {
 if(p->IsAlive()) {
 projTemp.push_back(p);
 }
 else{
 fire--;
 }
 }
 projectiles.clear();
 projectiles = list<shared_ptr<SFAsset>>(projTemp);
}



void SFApp::OnRender() {
  SDL_RenderClear(sf_window->getRenderer());


//set walls to be written
  for(auto w: walls){
    w->OnRender();
     }

// draw the player
  if(player->IsAlive()) {
  player->OnRender();
 }

gameover->OnRender();

// draw projectiles
 for(auto p: projectiles) {
 if(p->IsAlive()) {
 p->OnRender();
}
}

// draw healthpacks
 for(auto hp: healthpacks) {
 auto hpPos = hp->GetPosition();

 // if the player gains 500 points - spawn a health pack
 if( HealthPackSeed > 500){
 HealthPackSeed = HealthPackSeed - 500;
 int w, h;
 SDL_GetRendererOutputSize(sf_window->getRenderer(),&w,&h);
 auto pos = Point2 (rand() % (500), 600);
 hp->SetPosition(pos);
 hp->SetHealthPackAlive();
 }

 // render the alive healthpacks
 else if(hp->IsAlive()&& !(hpPos.getY()< 0)) {
 hp->OnRender();
 }
 else {
 // after the initial health pack is spawned, hold the health pack off screen until player gains sufficient points.
 int w, h;
 SDL_GetRendererOutputSize(sf_window->getRenderer(),&w,&h);
 auto pos = Point2 (-10, -10);
 hp->SetPosition(pos);
 hp->SetHealthPackAlive();
 }
 }

// aliens generation
  for(auto a: aliens) {
    auto aPos = a->GetPosition();
    if(a->IsAlive() && !(aPos.getY() < -30.0f)) {
      a->OnRender();
    }
    else {
      int w, h;
      SDL_GetRendererOutputSize(sf_window->getRenderer(),&w,&h);
      auto pos = Point2 (rand() % (40 + 600), 3000);
      a->SetPosition(pos);
      a->SetAlienAlive();
    }
  }

// generation of alien missiles
	for(auto d: alienmissiles) {
  auto dPos = d->GetPosition();
  if(d->IsAlive() && !(dPos.getY() < -30.0f)) {
  d->OnRender(); }
  else {
  int w, h;
  SDL_GetRendererOutputSize(sf_window->getRenderer(),&w,&h);
  auto pos = Point2 (rand() % (40 + 600), 3000);
  d->SetPosition(pos);
  d->SetAlienMissileAlive();
}
}

// generation of coin 
for(auto c: coins) {
auto cPos = c->GetPosition();
if(c->IsAlive() && !(cPos.getY() < -30.0f)) {
c->OnRender(); }
else {
int w, h;
SDL_GetRendererOutputSize(sf_window->getRenderer(),&w,&h);
auto pos = Point2 (rand() % (40 + 600), 3000);
c->SetPosition(pos);
c->SetCoinAlive();
}
}

//star generation
 for(auto s: stars) {
 auto sPos = s->GetPosition();
 if(s->IsAlive() && !(sPos.getY() < -30.0f))
 {
 s->OnRender(); }
 else {
 int w, h;
 SDL_GetRendererOutputSize(sf_window->getRenderer(),&w,&h);
 auto pos = Point2 (rand() % (40 + 600), 700);
 s->SetPosition(pos);
 s->SetStarAlive();
 }
 }

  // Switch the off-screen buffer to be on-screen
  SDL_RenderPresent(sf_window->getRenderer());
}

void SFApp::FireProjectile() {
auto pb = make_shared<SFAsset>(SFASSET_PROJECTILE, sf_window);
auto v  = player->GetPosition();
pb->SetPosition(v);
projectiles.push_back(pb);
}

void SFApp::GameOver(){
 cout <<"You Died!" << endl;
 cout <<"Your Final Score was: " << Points << endl;
 auto go_pos = Point2(320,480);
 gameover->SetPosition(go_pos);
is_paused=true;
}

void SFApp::Pause(){
 if(is_paused){
 is_paused = false;
 }
 else{
 is_paused = true;
 }
}
