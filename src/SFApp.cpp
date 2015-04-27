#include "SFApp.h"

SFApp::SFApp(std::shared_ptr<SFWindow> window) : fire(0), is_running(true), sf_window(window) {
  int canvas_w, canvas_h;
  SDL_GetRendererOutputSize(sf_window->getRenderer(), &canvas_w, &canvas_h);

  app_box = make_shared<SFBoundingBox>(Vector2(canvas_w, canvas_h), canvas_w, canvas_h);
  player  = make_shared<SFAsset>(SFASSET_PLAYER, sf_window);
  auto player_pos = Point2(canvas_w/2, 88.0f);
  player->SetPosition(player_pos);

  const int number_of_aliens = 10;
  for(int i=0; i<number_of_aliens; i++) {
    // place an alien at width/number_of_aliens * i
    auto alien = make_shared<SFAsset>(SFASSET_ALIEN, sf_window);
    //auto pos = Point2 (rand() % (canvas_w-50), rand() %(canvas_h + 200));
auto pos = Point2 (rand() % (20 + 500), 550);
    alien->SetPosition(pos);
    aliens.push_back(alien);
  }

auto debris = make_shared<SFAsset>(SFASSET_DEBRIS, sf_window);
 auto debris_pos = Point2 (rand() % (20 + 500), 550);
 debris->SetPosition(debris_pos);
debrise.push_back(debris);


  auto coin = make_shared<SFAsset>(SFASSET_COIN, sf_window);
  auto pos  = Point2(320.0f, 550.0f);
  coin->SetPosition(pos);
  coins.push_back(coin);
}

SFApp::~SFApp() {
}

/**
 * Handle all events that come from SDL.
 * These are timer or keyboard events.
 */
void SFApp::OnEvent(SFEvent& event) {
  SFEVENT the_event = event.GetCode();
  switch (the_event) {
  case SFEVENT_QUIT:
    is_running = false;
    break;
  case SFEVENT_UPDATE:
    OnUpdateWorld();
    OnRender();
    break;
  case SFEVENT_PLAYER_LEFT:
    player->GoWest();
    break;
  case SFEVENT_PLAYER_RIGHT:
    player->GoEast();
    break;
 case SFEVENT_PLAYER_UP:
    player->GoNorth();
    break;
  case SFEVENT_PLAYER_DOWN:
    player->GoSouth();
    break;
  case SFEVENT_FIRE:
    fire ++;
    FireProjectile();
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
  // Update projectile positions
  for(auto p: projectiles) {
    p->GoNorth();
  }

// coin movement and collision
  for(auto c: coins) {
    c->CoinN();
	if(player->CollidesWith(c)) {
	c->HandleCollision(); }
  }

//Debris movement and collision

	for(auto d: debrise){
	d->DebrisN();
	}

  // Update enemy positions
  for(auto a : aliens) {
    a->AlienN();
  }

// Detection of projectile collisions with aliens
  for(auto p : projectiles) {
    for(auto a : aliens) {
      if(p->CollidesWith(a)) {
        p->HandleCollision();
        a->HandleCollision();
      }
    }
// Detection of projectile collisions with debris
	for(auto d : debrise){
	if(p->CollidesWith(d)){
	 p->HandleCollision();
	d->HandleCollision();
}
 }
  }

  // remove dead aliens (the long way)
 // list<shared_ptr<SFAsset>> tmp;
//  for(auto a : aliens) {
//    if(a->IsAlive()) {
//      tmp.push_back(a);
//    }
//  }
//  aliens.clear();
 // aliens = list<shared_ptr<SFAsset>>(tmp);
}

void SFApp::OnRender() {
  SDL_RenderClear(sf_window->getRenderer());

  // draw the player
  player->OnRender();

  for(auto p: projectiles) {
    if(p->IsAlive()) {p->OnRender();}
  }

// aliens generation

  for(auto a: aliens) {
auto aPos = a->GetPosition();
if(a->IsAlive() && !(aPos.getY() < -30.0f)) {
a->OnRender(); }
else {
int w, h;
SDL_GetRendererOutputSize(sf_window->getRenderer(),&w,&h);
auto pos = Point2 (rand() % (20 + 500), 550);
a->SetPosition(pos);
a->SetAlienAlive();
}}

// generation of debris

	for(auto d: debrise) {
auto dPos = d->GetPosition();
if(d->IsAlive() && !(dPos.getY() < -30.0f)) {
d->OnRender(); }

else {
 int w, h;
 SDL_GetRendererOutputSize(sf_window->getRenderer(),&w,&h);
auto pos = Point2 (rand() % (500), 550);
d->SetPosition(pos);
d->SetDebrisAlive();
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
auto pos = Point2 (rand() % (20 + 500), 550);
c->SetPosition(pos);
c->SetCoinAlive();
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
