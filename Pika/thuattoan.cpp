#include <bits/stdc++.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include<SDL_ttf.h>
#include <ctime>

using namespace std;

const int SCREEN_WIDTH = 1100;
const int SCREEN_HEIGHT = 750;
const int row = 18;
const int collum = 11;
const int h_w = 50;
int const_row = 2 * h_w;
int const_collum = h_w;

const string WINDOW_TITLE = "PIKACHU";

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
  //window = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_FULL_DESKTOP);
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

void renderTexture(SDL_Texture* texture, SDL_Renderer* renderer, int x, int y, int w, int h)
{

  SDL_Rect rect;
  rect.x = x;
  rect.y = y;
  rect.w = w;
  rect.h = h;
  //SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
  SDL_RenderCopy(renderer, texture, NULL, &rect);
}

void renderTexture(SDL_Texture* texture, SDL_Renderer* renderer, int x, int y)
{

  SDL_Rect rect;
  rect.x = x;
  rect.y = y;
  //SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
  SDL_RenderCopy(renderer, texture, NULL, &rect);
}

void background(const string &file)
{
  //initSDL(window, renderer);
  SDL_RenderClear(renderer);

  SDL_Texture *background = loadTexture(file.c_str(), renderer);
  if(background == nullptr) {
    SDL_DestroyTexture(background);
    quitSDL(window, renderer);
    //break;
  }

  SDL_Rect rect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
  SDL_RenderCopy(renderer, background, NULL, &rect);

  SDL_RenderPresent(renderer);
}

int arr[collum][row] = {0};
string photos[25];

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

  for(int i = 1; i < 25; i++)
  {
    for(int j = 1; j < 7; j++)
      {
        val.push_back(i);
      }
  }

  random_shuffle(val.begin(), val.end());
  random_shuffle(point.begin(), point.end());

  int n = val.size();
  for(int i = 0; i < n; i++)
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
  bool check1, check2;
  for(int i = max(A.x, B.x); i >= max(minX_A, minX_B); --i)
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
  for(int i = max(A.x, B.x); i <= min(maxX_A, maxX_B); ++i)
  {
    check2 = true;
    for(int j = min(A.y, B.y) + 1; j <= max(A.y, B.y) - 1; ++j)
    {
      if(arr[i][j] != 0) check2 = false;
    }
    if(check2)
    {
      st_B.push({i, A.y});
      st_A.push({i, B.y});
    }
    if(st_B.size() != 0) return st_B;
  }
  //cout << minX_A << " " << maxX_A << " " << minX_B << " " << maxX_B << endl;
  //cout << st_A.size() << " " << st_B.size() << endl;
  if(st_A.size() != 0) return st_A;
  else return st_B;
  //return true;
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
  bool check1, check2;
  for(int i = max(A.y, B.y); i >= max(minY_A, minY_B); --i)
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
  for(int i = max(A.y, B.y); i <= min(maxY_A, maxY_B); ++i)
  {
    check2 = true;
    for(int j = min(A.x, B.x) + 1; j <= max(A.x, B.x) - 1; ++j)
    {
      if(arr[j][i] != 0) check2 = false;
    }
    if(check2)
    {
      st_B.push({A.x, i});
      st_B.push({B.x, i});
    }
    if(st_B.size() != 0) return st_B;
  }
  //cout << minY_A << " " << maxY_A << " " << minY_B << " " << maxY_B << endl;
  //3cout << st_A.size() << " " << st_B.size() << endl;
  if(st_A.size() != 0) return st_A;
  else return st_B;
  //return true;
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
    arr[A.x][A.y] = 0;
    arr[B.x][B.y] = 0;
  }
}

//chọn tọa độ của hình
Point get_move()
{
  Point point;
  cout << "Enter your pick: " ;
  cin >> point.x >> point.y;
  return point;
}

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
  background("photos/background.jpg");
  SDL_Rect rect;
  rect.w = h_w;
  rect.h = h_w;

  for(int j = 1; j <= collum - 2; ++ j){
    for(int i = 1; i <= row - 2; ++i){
      rect.x = (i - 1) * h_w + const_collum;
      rect.y = (j - 1) * h_w + const_row;

      if(arr[j][i] > 0)
      {
        if((i + j) % 2 == 1) {
          SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
          SDL_RenderDrawRect(renderer, &rect);
        }
        else{
          SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
          SDL_RenderDrawRect(renderer, &rect);
        }
        string s = "photos/" + to_string(arr[j][i]) + ".0.png";
        SDL_Texture *abc = loadTexture(s, renderer);
        SDL_RenderCopy(renderer, abc, NULL, &rect);
      }

      //SDL_Delay(10);
      SDL_RenderPresent(renderer);
    }
  }
}

//dùng để in hình ảnh background tại vị trí thay đổi
void printRect(Point A)
{
  SDL_Rect rect;
  rect.w = h_w;
  rect.h = h_w;

  rect.x = (A.y - 1) * h_w + const_collum;
  rect.y = (A.x - 1) * h_w + const_row;
  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
  SDL_RenderDrawRect(renderer, &rect);
  string s = "photos/" + to_string(A.x) + "." + to_string(A.y) + ".png";
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
//trả về địa chỉ của con trỏ vừa thực hiện
Point mouse()
{
  SDL_Event e;
  while(SDL_WaitEvent(&e))
  {
    if(e.type == SDL_QUIT)
    {
      SDL_Quit();
      break;
    }
    if(e.type == SDL_MOUSEBUTTONUP)
    {
      int x = e.button.x;
      int y = e.button.y;
      if(x >= 0 && y >= 0 && y <= SCREEN_HEIGHT && x <= SCREEN_WIDTH)
      {
        x -= const_collum;
        x = (x + h_w - 1) / h_w;
        y -= const_row;
        y = (y + h_w - 1) / h_w;
      }
      if(x >= 1 && y >= 1 && y <= collum - 2 && x <= row - 2)
      {
        return {y, x};
        break;
      }
    }
  }
}


//chạy chương trình
void play_game()
{
  do
  {
    //printMap();
    Point turn1 = mouse();
    Point turn2 = mouse();

    if(checkX(turn1, turn2).size() != 0 || checkY(turn1, turn2).size() != 0)
    {
      checkPoint(turn1, turn2);
      printPoint(turn1, turn2);

    }
    if(isGameOver())
    {
      cout << "You are winner!" << endl;
      background("photos/win.png");
      break;
    }
    if(turn1.x * turn1.y * turn2.x * turn2.y == 0) continue;
  } while(!isGameOver());
}


int main(int argc, char* argv[])
{
  ios_base::sync_with_stdio(false);

  initSDL(window, renderer);
  SDL_RenderClear(renderer);
  //SDL_SetRenderDrawColor(renderer,255,0,0,255);
  //SDL_RenderPresent(renderer);

  makeArr();
  printMap();
  play_game();

  SDL_RenderPresent(renderer);

  waitUntilKeyPressed();
  quitSDL(window, renderer);
  return 0;
}
