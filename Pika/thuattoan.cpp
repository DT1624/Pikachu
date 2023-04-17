#include <bits/stdc++.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <ctime>

using namespace std;

const int SCREEN_WIDTH = 1100;
const int SCREEN_HEIGHT = 750;
const int row = 18;
const int collum = 11;
const int h_w = 50;
int const_row = 2 * h_w;
int const_collum = 3 * h_w / 2;
bool check = true;

const string WINDOW_TITLE = "PIKACHU";

int arr[collum][row] = {0};
string photos[25];
SDL_Window* window;
SDL_Renderer* renderer;

void logSDLError(ostream& os, const string &msg, bool fatal = false)
{
  os << msg << "Error: " << SDL_GetError() << endl;
  if(fatal)
  {
    SDL_Quit();
    exit(1);
  }
}

void initSDL(SDL_Window* &window, SDL_Renderer* &renderer)
{
  if(SDL_Init(SDL_INIT_EVERYTHING) != 0) logSDLError(cout, "SDL_Init", true);

  window = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
  //window = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_FULLSCREEN_DESKTOP);
  if(window == nullptr) logSDLError(cout, "CreateWindow", true);

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
  SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void quitSDL(SDL_Window* window, SDL_Renderer* renderer)
{
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

void waitUntilKeyPressed()
{
  SDL_Event e;
  while(true)
  {
    if(SDL_WaitEvent(&e) != 0 && (e.type == SDL_KEYDOWN || e.type == SDL_QUIT)) return;
    SDL_Delay(100);
  }
}

SDL_Texture* loadTexture(string path, SDL_Renderer* renderer){
    SDL_Texture* newTexture = nullptr;
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if(loadedSurface == nullptr){
        cout<<"Unable to  load image " << path << " SDL_image Error: "
        << IMG_GetError() << endl;
    }
    else{
        newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        if(newTexture == nullptr)
            cout<<"Unable to create texture from " << path << " SDL Error: "
                << SDL_GetError()<< endl;
                SDL_FreeSurface( loadedSurface);
    }
    return newTexture;
}

void loadFont_start()
{
	bool isRunning = true;
	SDL_Event e;
	TTF_Font* font = NULL;
	SDL_Surface* surface = NULL;
	SDL_Texture* texture = NULL;

	//Initialize the truetype font API.
	if (TTF_Init() < 0)
	{
		SDL_Log("%s", TTF_GetError());
		SDL_Quit();
	}

	font = TTF_OpenFont("font/verdanab.ttf", 120);
	SDL_Color fg = {255, 0, 255};
  string text = "PIKACHU";

	surface = TTF_RenderText_Solid(font, text.c_str(), fg);
	texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	SDL_Rect srcRest;
	SDL_Rect desRect;

  TTF_SizeText(font, text.c_str(), &srcRest.w, &srcRest.h);
	srcRest.x = 0;
	srcRest.y = 0;

	desRect.x = 260;
	desRect.y = 300;

	desRect.w = srcRest.w;
	desRect.h = srcRest.h;
	//SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

  SDL_RenderCopy(renderer, texture, &srcRest, &desRect);
  SDL_RenderPresent(renderer);
}

void loadFont_word(string path, int x, int y)
{
	TTF_Font* font = NULL;
	SDL_Surface* surface = NULL;
	SDL_Texture* texture = NULL;

	//Initialize the truetype font API.
	if (TTF_Init() < 0)
	{
		SDL_Log("%s", TTF_GetError());
		SDL_Quit();
	}

	font = TTF_OpenFont("font/VNCOOP.ttf", 50);
	SDL_Color fg = {0, 0, 255};
  //string text = "PIKACHU";

	surface = TTF_RenderText_Solid(font, path.c_str(), fg);
	texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	SDL_Rect srcRest;
	SDL_Rect desRect;

  TTF_SizeText(font, path.c_str(), &srcRest.w, &srcRest.h);
	srcRest.x = 0;
	srcRest.y = 0;
  //cout << srcRest.w << " " << srcRest.h << endl;
	desRect.x = x;
	desRect.y = y;

	desRect.w = 130;
	desRect.h = 50;
	//SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderCopy(renderer, texture, NULL, &desRect);
  SDL_RenderPresent(renderer);
}

void loadFont_number(string path, int x, int y)
{
	TTF_Font* font = NULL;
	SDL_Surface* surface = NULL;
	SDL_Texture* texture = NULL;

	//Initialize the truetype font API.
	if (TTF_Init() < 0)
	{
		SDL_Log("%s", TTF_GetError());
		SDL_Quit();
	}

	font = TTF_OpenFont("font/ariblk.ttf", 40);
	SDL_Color fg = {255, 255, 0};
  //string text = "PIKACHU";

	surface = TTF_RenderText_Solid(font, path.c_str(), fg);
	texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	SDL_Rect srcRest;
	SDL_Rect desRect;

  TTF_SizeText(font, path.c_str(), &srcRest.w, &srcRest.h);
	srcRest.x = 0;
	srcRest.y = 0;
  //cout << srcRest.w << " " << srcRest.h << endl;
	desRect.x = x;
	desRect.y = y;

	desRect.w = 45;
	desRect.h = 60;
	//SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

  SDL_RenderCopy(renderer, texture, NULL, &desRect);
  SDL_RenderPresent(renderer);
}

void background(const string &file)
{
  SDL_RenderClear(renderer);

  SDL_Texture *background = loadTexture(file.c_str(), renderer);
  if(background == nullptr) {
    SDL_DestroyTexture(background);
    quitSDL(window, renderer);
    //break;
  }
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
  SDL_Rect rect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
  SDL_RenderCopy(renderer, background, NULL, &rect);

  SDL_RenderPresent(renderer);
}

//tạo các điểm
struct Point
{
  public:
  int x;
  int y;
};

//đọc mảng từ file đã có
void makeArr()
{
  srand(time(NULL));

  vector<Point> point;
  vector<int> val;

  for(int i = 1; i <= collum - 2; i++)
  {
    for(int j = 1; j <= row - 2; j++)
    {
      point.push_back({i, j});
    }
  }

  for(int i = 1; i <= 36; i++)
  {
    for(int j = 1; j <= 4; j++)
      {
        val.push_back(i);
      }
  }

  random_shuffle(val.begin(), val.end());
  random_shuffle(val.begin(), val.end());
  //random_shuffle(val.begin(), val.end());
  //random_shuffle(point.begin(), point.end());

  int n = val.size();
  for(int i = 0; i < n; i++)
  {
      arr[point[i].x][point[i].y] = val[i];
  }
//  ifstream file("matran.txt");
//  for(int i = 1; i <= collum - 2; ++i)
//  {
//    for(int j = 1; j <= row - 2; ++j)
//    {
//      //file << arr[i][j] << " ";
//      file >> arr[i][j] ;
//      //cout << arr[i][j] << " ";
//    }
//    //file << endl;
//    //file >> endl;
//  }
////  for(int i = 0; i < collum; ++i)
////  {
////    for(int j ; j <= row - 2; ++j)
////    {
////      //file >> arr[i][j] ;
////      cout << arr[i][j];
////      if(arr[i][j] < 10) cout << "  ";
////      else cout << " ";
////    }
////    cout << endl;
////  }
}


void random()
{
  vector<Point> point;
  vector<int> val;

  for(int i = 1; i <= collum - 2; i++)
  {
    for(int j = 1; j <= row - 2; j++)
    {
      if(arr[i][j] > 0)
      {
        point.push_back({i, j});
        val.push_back(arr[i][j]);
      }
    }
  }

  random_shuffle(val.begin(), val.end());
  random_shuffle(val.begin(), val.end());

  for(int i = 0; i < point.size(); ++i)
  {
      arr[point[i].x][point[i].y] = val[i];
  }
}

//check các đường đi theo chiều Õ
stack<Point> checkX(Point A, Point B) {
  stack<Point> st_A, st_B;
  //nếu 2 ô cùng cột thì ta sẽ xét đường đi theo hàng
  if(A.y == B.y || arr[A.x][A.y] != arr[B.x][B.y] || arr[A.x][A.y] * arr[B.x][B.y] == 0) return st_A;
  int minX_A = A.x, maxX_A = A.x;
  int minX_B = B.x, maxX_B = B.x;
  for(int i = A.x - 1; i >= 0; --i)
  {
    if(arr[i][A.y] == 0) minX_A = i;
    else break;
  }
  for(int i = B.x - 1; i >= 0; --i)
    {
    if(arr[i][B.y] == 0) minX_B = i;
    else break;
  }
  for(int i = A.x + 1; i < collum; ++i)
  {
    if(arr[i][A.y] == 0) maxX_A = i;
    else break;
  }
  for(int i = B.x + 1; i < collum; ++i)
  {
    if(arr[i][B.y] == 0) maxX_B = i;
    else break;
  }
  if(minX_A > maxX_B || maxX_A < minX_B) return st_A;
  bool check1;
  int min_max = min(maxX_A, maxX_B);
  int max_min = max(minX_A, minX_B);
  for(int i = min(min_max, max_min); i <= max(min_max, max_min); ++i)
  {
    check1 = true;
    for(int j = min(A.y, B.y) + 1; j <= max(A.y, B.y) - 1; ++j)
    {
      if(arr[i][j] != 0) check1 = false;
    }
    if(check1)
    {
      st_A.push({i, A.y});
      st_A.push({i, B.y});
    }
    if(st_A.size() != 0) return st_A;
  }
  if(st_A.size() != 0) return st_A;
  else return st_B;
}

//check các đường đi theo chiều Oy
stack<Point> checkY(Point A, Point B) {
  stack<Point> st_A, st_B;
  //nếu 2 ô cùng hàng thì ta sẽ xét đường đi theo cột
  if(A.x == B.x || arr[A.x][A.y] != arr[B.x][B.y] || arr[A.x][A.y] * arr[B.x][B.y] == 0) return st_A;
  int minY_A = A.y, maxY_A = A.y;
  int minY_B = B.y, maxY_B = B.y;
  for(int i = A.y - 1; i >= 0; --i)
  {
    if(arr[A.x][i] == 0) minY_A = i;
    else break;
  }
  for(int i = B.y - 1; i >= 0; --i)
    {
    if(arr[B.x][i] == 0) minY_B = i;
    else break;
  }
  for(int i = A.y + 1; i < row; ++i)
  {
    if(arr[A.x][i] == 0) maxY_A = i;
    else break;
  }
  for(int i = B.y + 1; i < row; ++i)
  {
    if(arr[B.x][i] == 0) maxY_B = i;
    else break;
  }
  if(minY_A > maxY_B || maxY_A < minY_B) return st_A;
  bool check1;
  int min_max = min(maxY_A, maxY_B);
  int max_min = max(minY_A, minY_B);
  for(int i = min(min_max, max_min); i <= max(min_max, max_min); ++i)
  {
    check1 = true;
    for(int j = min(A.x, B.x) + 1; j <= max(A.x, B.x) - 1; ++j)
    {
      if(arr[j][i] != 0) check1 = false;
    }
    if(check1)
    {
      st_A.push({A.x, i});
      st_A.push({B.x, i});
    }
    if(st_A.size() != 0) return st_A;
  }
  if(st_A.size() != 0) return st_A;
  else return st_B;
}

//check đường đi giữa 2 điểm
//nếu có đường đi thì xóa cả 2 ngược lại thì không
void checkPoint(Point &A, Point &B)
{
  stack<Point> s2 = checkY(A, B);
  stack<Point> s1 = checkX(A,B);
  if(arr[A.x][A.y] != arr[B.x][B.y]) return;
  stack<Point> s;
  if(checkX(A, B).size() != 0)
  {
    s = checkX(A, B);
  }
  else
  {
    s = checkY(A, B);
  }

  if(s.size() != 0)
  {
//    while(!s.empty()) {
//      cout << s.top().x << " " << s.top().y << endl;
//      s.pop();
//    }
    arr[A.x][A.y] = 0;
    arr[B.x][B.y] = 0;
  }
}

bool check_Exit_Pair()
{
  vector<Point> point;
  for(int i = 1; i <= collum - 2; i++)
  {
    for(int j = 1; j <= row - 2; j++)
    {
      if(arr[i][j] > 0 ) point.push_back({i, j});
    }
  }

  for(int i = 0; i < point.size(); i++)
  {
    for(int j = i + 1; j < point.size(); j++)
    {
      Point A = {point[i].x, point[i].y};
      Point B = {point[j].x, point[j].y};
      if(checkX(A, B).size() != 0 || checkY(A, B).size() != 0) return true;;
    }
  }
  return false;
}

////chọn tọa độ của hình
//Point get_move()
//{
//  Point point;
//  cout << "Enter your pick: " ;
//  cin >> point.x >> point.y;
//  return point;
//}

//kiểm tra bảng đã hết hay chưa
bool isGameOver() {
  for(int i = 0; i < collum; ++i)
  {
    for(int j = 0; j < row; ++j)
    {
      if(arr[i][j] != 0) return false;
    }
  }
  return true;
}

//in map tại thời điểm hiên tại
void printMap()
{
  SDL_RenderClear(renderer);
  //initSDL(window, renderer);
  background("photos/b1.jpg");
  SDL_Rect rect;
  rect.w = h_w;
  rect.h = h_w;


  for(int j = 1; j <= collum - 2; ++ j){
    for(int i = 1; i <= row - 2; ++i){
      rect.x = (i - 1) * h_w + const_collum;
      rect.y = (j - 1) * h_w + const_row;

      if(arr[j][i] > 0)
      {
        SDL_SetRenderDrawColor(renderer, 223, 103, 83, 0);
        SDL_RenderFillRect(renderer, &rect);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawRect(renderer, &rect);

        SDL_Rect rect1;
        rect1.w = h_w - 10;
        rect1.h = h_w - 10;
        rect1.x = (i - 1) * h_w + const_collum + 5;
        rect1.y = (j - 1) * h_w + const_row + 5;
        string s = "1/" + to_string(arr[j][i]) + ".3.png";
        SDL_Texture *abc = loadTexture(s, renderer);
        SDL_RenderCopy(renderer, abc, NULL, &rect1);
        //SDL_RenderPresent(renderer);
      }

    }
  }
  SDL_RenderPresent(renderer);
  SDL_Rect rect1;
  rect1.w = h_w;
  rect1.h = h_w;
  rect1.x = 200;
  rect1.y = 600;
  SDL_SetRenderDrawColor(renderer, 223, 103, 83, 0);
  SDL_RenderFillRect(renderer, &rect1);
  SDL_Texture *abc = loadTexture("photos/random.png", renderer);
  SDL_RenderCopy(renderer, abc, NULL, &rect1);

  loadFont_word("SCORE", 920, 100);
  loadFont_word("TURN", 920, 260);
  loadFont_word("MAP", 920, 420);
  SDL_RenderPresent(renderer);
}

void print_random()
{
  SDL_Rect rect;
  rect.w = h_w;
  rect.h = h_w;


  for(int j = 1; j <= collum - 2; ++ j){
    for(int i = 1; i <= row - 2; ++i){
      rect.x = (i - 1) * h_w + const_collum;
      rect.y = (j - 1) * h_w + const_row;

      if(arr[j][i] > 0)
      {
        SDL_SetRenderDrawColor(renderer, 223, 103, 83, 0);
        SDL_RenderFillRect(renderer, &rect);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawRect(renderer, &rect);

        SDL_Rect rect1;
        rect1.w = h_w - 10;
        rect1.h = h_w - 10;
        rect1.x = (i - 1) * h_w + const_collum + 5;
        rect1.y = (j - 1) * h_w + const_row + 5;
        string s = "1/" + to_string(arr[j][i]) + ".3.png";
        SDL_Texture *abc = loadTexture(s, renderer);
        SDL_RenderCopy(renderer, abc, NULL, &rect1);
      }
    }
  }
  SDL_RenderPresent(renderer);
}
//trả về địa chỉ của con trỏ vừa thực hiện
Point mouse()
{
  SDL_Event e;
  while(SDL_WaitEvent(&e))
  {
    if(e.type == SDL_QUIT)
    {
      check = false;
      SDL_Quit();
      break;
    }
    if(e.type == SDL_MOUSEBUTTONUP)
    {
      int x = e.button.x;
      int y = e.button.y;
      if(x >= 0 && y >= 0 && y <= SCREEN_HEIGHT && x <= SCREEN_WIDTH)
      {
        if(x >= 200 && x <= 250 && y >= 600 && y <= 650) {
          return {11, 4};
          break;
        }
        x -= const_collum;
        x = (x + h_w - 1) / h_w;
        y -= const_row;
        y = (y + h_w - 1) / h_w;
        if(x >= 1 && y >= 1 && y <= collum - 2 && x <= row - 2 && arr[y][x] > 0)
        {
          return {y, x};
          break;
        }
      }

    }
  }
}

bool mouse_random(Point A)
{
  if(A.x == 11 && A.y == 4) return true;
  return false;
}

//dùng để in hình ảnh background tại vị trí thay đổi
void printRect(Point A)
{
  SDL_Rect rect;
  rect.w = h_w;
  rect.h = h_w;

  rect.x = (A.y - 1) * h_w + const_collum;
  rect.y = (A.x - 1) * h_w + const_row;

  string s = "photos/" + to_string(A.x) + "." + to_string(A.y) + ".png";
  SDL_Texture *abc = loadTexture(s, renderer);
  SDL_RenderCopy(renderer, abc, NULL, &rect);
  SDL_RenderPresent(renderer);
}

void printBefore(Point A)
{
  SDL_Rect rect;
  rect.w = h_w;
  rect.h = h_w;

  rect.x = (A.y - 1) * h_w + const_collum;
  rect.y = (A.x - 1) * h_w + const_row;

  SDL_SetRenderDrawColor(renderer, 223, 103, 83, 0);
  SDL_RenderFillRect(renderer, &rect);
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderDrawRect(renderer, &rect);

  rect.w -= 10;
  rect.h -= 10;
  rect.x += 5;
  rect.y += 5;
  string s = "1/" + to_string(arr[A.x][A.y]) + ".3.png";
  SDL_Texture *abc = loadTexture(s, renderer);
  SDL_RenderCopy(renderer, abc, NULL, &rect);
  SDL_RenderPresent(renderer);
}

void printAfter(Point A)
{
  SDL_Rect rect;
  rect.w = h_w;
  rect.h = h_w;

  rect.x = (A.y - 1) * h_w + const_collum;
  rect.y = (A.x - 1) * h_w + const_row;

  SDL_SetRenderDrawColor(renderer, 120, 75, 230, 0);
  SDL_RenderFillRect(renderer, &rect);
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderDrawRect(renderer, &rect);

  rect.w -= 10;
  rect.h -= 10;
  rect.x += 5;
  rect.y += 5;
  string s = "1/" + to_string(arr[A.x][A.y]) + ".3.png";
  SDL_Texture *abc = loadTexture(s, renderer);
  SDL_RenderCopy(renderer, abc, NULL, &rect);
  SDL_RenderPresent(renderer);

}

// chỉ in vị trí có thay đổi
void printPoint(Point A, Point B)
{
  if(arr[A.x][A.y] == 0 && arr[B.x][B.y] == 0)
  {
    printRect(A);
    printRect(B);
  }

}


void printBoard()
{
  for(int i = 0; i < collum; ++i)
  {
    for(int j = 0; j < row; ++j)
    {
      cout << arr[i][j] << " ";
      if(arr[i][j] < 10) cout << "  ";
      else cout << " ";
    }
    cout << endl;
  }
  cout << "---------------------------" << endl << endl;
}
//chạy chương trình
void play_game()
{
  int score = 0;
  do
  {

    if(!check_Exit_Pair())
    {
      random();
      printMap();
    }
    SDL_Rect rect1;
    rect1.w = 130;
    rect1.h = 60;
    rect1.x = 920;
    rect1.y = 150;
    //SDL_SetRenderDrawColor(renderer, 223, 103, 83, 0);
    //SDL_RenderFillRect(renderer, &rect1);
    SDL_Texture *abc1 = loadTexture("photos/bgr_score.jpg", renderer);
    SDL_RenderCopy(renderer, abc1, NULL, &rect1);
    SDL_RenderPresent(renderer);
    string s_score = to_string(score);
    loadFont_number(s_score, 960, 150);
    Point turn1 = mouse();
    if(mouse_random(turn1)) {
      random();
      print_random();
      continue;
    }
    else if(check) printAfter(turn1);
    Point turn2 = mouse();
    if(mouse_random(turn2)) {
      random();
      print_random();
      continue;
    }

    if(checkX(turn1, turn2).size() != 0 || checkY(turn1, turn2).size() != 0)
    {
      checkPoint(turn1, turn2);
      printPoint(turn1, turn2);
      score += 10;
    }
    else if(check)
    {
      printBefore(turn1);
      printBefore(turn2);
    }
    if(isGameOver())
    {
      cout << "You are winner!" << endl;
      background("photos/win.png");
      break;
    }
  } while(!isGameOver());
}

void start()
{
  SDL_RenderClear(renderer);

  SDL_Texture *background = loadTexture("photos/background_start.png", renderer);
  SDL_Texture *start= loadTexture("photos/start.png", renderer);
  if(background == nullptr || start == nullptr) {
    SDL_DestroyTexture(background);
    SDL_DestroyTexture(start);
    quitSDL(window, renderer);
  }

  SDL_Rect rect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
  SDL_RenderCopy(renderer, background, NULL, &rect);

  SDL_Rect rect1;
  rect1.x = 345;
  rect1.y = 450;
  rect1.w = 460;
  rect1.h = 180;
  SDL_RenderCopy(renderer, start, NULL, &rect1);

  SDL_RenderPresent(renderer);

  loadFont_start();

  SDL_Event e;
  while(SDL_WaitEvent(&e))
  {
    if(e.type == SDL_QUIT)
    {
      SDL_Quit();
      //break;
    }
    if(e.type == SDL_MOUSEBUTTONUP)
    {
      int x = e.button.x;
      int y = e.button.y;
      if(x >= rect1.x && y >= rect1.y && y <= rect1.y + rect.h && x <= rect1.x + rect1.w)
      {
        printMap();
        play_game();
      }
    }
  }
}

int main(int argc, char* argv[])
{
  ios_base::sync_with_stdio(false);

  initSDL(window, renderer);
  SDL_RenderClear(renderer);

  makeArr();
  start();

  SDL_RenderPresent(renderer);

  waitUntilKeyPressed();
  quitSDL(window, renderer);
  return 0;
}
