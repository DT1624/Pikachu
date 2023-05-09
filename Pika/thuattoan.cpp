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
const int column = 11;
const int h_w = 50;
const string WINDOW_TITLE = "PIKACHU";
const int FRAME_DELAY = 50;
const int FPS = 20;
const int lv_time = 600;
const int levelMax = 9;

int arr[column][row] = {0};
string photos[25];
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
int const_row = 2 * h_w;
int const_column = 3 * h_w / 2;
bool check = true;
bool lose = false;
int score = 0;
int turn = 10;
int level = 1;
int sound = 0;
int time_lv = lv_time + 20 * (level - 1);;
Mix_Music *gMusic;
map<int, int> m;
int time_end[100] = {0};
int score_high[11];
int time_pause = time_lv;
int imenu = 0;


bool menu_score = false;
bool menu_play = false;
bool menu_intro = false;

bool game_change = false;
bool game_main = false;
bool game_resume = false;

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
    Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 );

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
    //SDL_RenderSetScale(renderer, 1920 / SCREEN_WIDTH, 1080 / SCREEN_HEIGHT);
}

void quitSDL(SDL_Window* window, SDL_Renderer* renderer)
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    IMG_Quit();
    TTF_Quit();
    Mix_Quit();
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

SDL_Texture* loadTexture(string path, SDL_Renderer* renderer)
{
    SDL_Texture* newTexture = nullptr;
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if(loadedSurface == nullptr)
    {
        cout<<"Unable to  load image " << path << " SDL_image Error: "
            << IMG_GetError() << endl;
    }
    else
    {
        newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        if(newTexture == nullptr)
            cout<<"Unable to create texture from " << path << " SDL Error: "
                << SDL_GetError()<< endl;
        SDL_FreeSurface( loadedSurface);
    }
    return newTexture;
    SDL_DestroyTexture(newTexture);
    SDL_FreeSurface(loadedSurface);
}

void renderTexture(string path, int x, int y, int w, int h)
{
    SDL_Rect rect = {x, y, w, h};
    SDL_Texture *file = loadTexture(path.c_str(), renderer);
    if(file == nullptr)
    {
        SDL_DestroyTexture(file);
        quitSDL(window, renderer);
        //break;
    }
    SDL_RenderCopy(renderer, file, NULL, &rect);
    SDL_DestroyTexture(file);
}

void drawCircle(SDL_Renderer* renderer, int x, int y, int r)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 25, 0);
    for(double i = -r / sqrt(2); i <= r / sqrt(2); i += 0.01)
    {
        SDL_RenderDrawPoint(renderer, x + i, y + sqrt(r * r - i * i));
        SDL_RenderDrawPoint(renderer, x + i, y - sqrt(r * r - i * i));
    }
    for (double i = -r / sqrt(2); i <= r / sqrt(2); i += 0.2)
    {
        SDL_RenderDrawPoint(renderer, x + sqrt(r * r - i * i), y + i);
        SDL_RenderDrawPoint(renderer, x - sqrt(r * r - i * i), y + i);
    }
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
}

void reset_img()
{
    SDL_Rect rect = {1025, 25, 50, 50};
    SDL_Texture *abc = loadTexture("photos/change.png", renderer);
    SDL_RenderCopy(renderer, abc, NULL, &rect);
    SDL_RenderPresent(renderer);
    SDL_DestroyTexture(abc);
}

void main_img()
{
    SDL_Rect rect = {950, 25, 50, 50};
    SDL_Texture *abc = loadTexture("photos/main.png", renderer);
    SDL_RenderCopy(renderer, abc, NULL, &rect);
    SDL_RenderPresent(renderer);
    SDL_DestroyTexture(abc);
}

void sound_img()
{
    SDL_Rect rect = {875, 25, 50, 50};
    SDL_Texture *abc;
    if(sound == 0) abc = loadTexture("photos/mix.png", renderer);
    else abc = loadTexture("photos/no_mix.png", renderer);
    SDL_RenderCopy(renderer, abc, NULL, &rect);
    SDL_RenderPresent(renderer);
    SDL_DestroyTexture(abc);
}

void mix()
{
    sound = (sound + 1) % 2;
    sound_img();
    if( Mix_PlayingMusic() == 0 )
    {
        Mix_PlayMusic( gMusic, -1 );
    }
    else
    {
        if( Mix_PausedMusic() == 1 )
        {
            Mix_ResumeMusic();
        }
        else
        {
            Mix_PauseMusic();
        }
    }
}

void print_random();

void sMenu()
{
    imenu = (imenu + 1) % 2;
    SDL_Texture *me = loadTexture("photos/menu.png", renderer);
    SDL_Rect rect = {960, 35, 100, 90};
    if(imenu == 1)
    {
        SDL_RenderCopy(renderer, me, NULL, &rect);
    }
    else
    {
        //SDL_RenderClear(renderer);
        print_random();
    }
    SDL_DestroyTexture(me);
}

void loadMedia()
{
    gMusic = Mix_LoadMUS( "sound/music.mp3" );
    if( gMusic == NULL )
    {
        printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
        Mix_Quit();
    }
    else Mix_PlayMusic(gMusic, -1);
}

void loadDelete()
{
    Mix_Chunk *gDelete = Mix_LoadWAV( "sound/delete.wav" );
    Mix_PlayChannel( -1, gDelete, 0 );
}

void loadNoDelete()
{
    Mix_Chunk *gNoDelete = Mix_LoadWAV( "sound/no_delete.wav" );
    Mix_PlayChannel( -1, gNoDelete, 0 );
}

void loadTurn()
{
    Mix_Chunk *gTurn = Mix_LoadWAV( "sound/turn.wav" );
    Mix_PlayChannel( -1, gTurn, 0 );
}
void loadFont_start()
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

    font = TTF_OpenFont("font/verdanab.ttf", 110);
    SDL_Color fg = {250, 10, 25};
    string text = "PIKACHU";

    surface = TTF_RenderText_Solid(font, text.c_str(), fg);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    SDL_Rect srcRest;
    SDL_Rect desRect;

    TTF_SizeText(font, text.c_str(), &srcRest.w, &srcRest.h);
    srcRest.x = 0;
    srcRest.y = 0;

    desRect.x = 285;
    desRect.y = 300;
    desRect.w = srcRest.w;
    desRect.h = srcRest.h;

    SDL_RenderCopy(renderer, texture, &srcRest, &desRect);
    SDL_RenderPresent(renderer);
    SDL_DestroyTexture(texture);
}

void loadFont(string path, int x, int y, int w, int h, int n)
{
    if (TTF_Init() < 0)
    {
        SDL_Log("%s", TTF_GetError());
        SDL_Quit();
    }

    TTF_Font *font = TTF_OpenFont("font/ariblk.ttf", 60);
    int a = 0, b, c = 255;
    if(n == 0)
    {
        b = 0;
    }
    else if(n == 1) b = 250;
    else
    {
        a = 20;
        b = 255;
        c = 25;
    }
    SDL_Color fg = {a, b, c};

    SDL_Surface *surface = TTF_RenderText_Solid(font, path.c_str(), fg);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    SDL_Rect srcRest;
    SDL_Rect desRect = {x, y, w, h};;

    TTF_SizeText(font, path.c_str(), &srcRest.w, &srcRest.h);
    srcRest.x = 0;
    srcRest.y = 100;

    SDL_RenderCopy(renderer, texture, NULL, &desRect);
    SDL_RenderPresent(renderer);
    font = NULL;
    SDL_DestroyTexture(texture);
}

void background(const string &file)
{
    SDL_RenderClear(renderer);

    SDL_Texture *background = loadTexture(file.c_str(), renderer);
    SDL_Rect rect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    //SDL_Rect rect = {75, 100, 875, 550};
    SDL_RenderCopy(renderer, background, NULL, &rect);

    SDL_RenderPresent(renderer);
    SDL_DestroyTexture(background);
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

    for(int i = 1; i <= column - 2; i++)
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

    int n = val.size();
    for(int i = 0; i < n; i++)
    {
        arr[point[i].x][point[i].y] = val[i];
    }
}

void random()
{
    vector<Point> point;
    vector<int> val;

    for(int i = 1; i <= column - 2; i++)
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


vector<pair<Point, Point>> checkX(Point A, Point B)
{
    vector<pair<Point, Point>> st_A;
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
    for(int i = A.x + 1; i < column; ++i)
    {
        if(arr[i][A.y] == 0) maxX_A = i;
        else break;
    }
    for(int i = B.x + 1; i < column; ++i)
    {
        if(arr[i][B.y] == 0) maxX_B = i;
        else break;
    }
    if(minX_A > maxX_B || maxX_A < minX_B) return st_A;
    bool check;
    int min_max = min(maxX_A, maxX_B);
    int max_min = max(minX_A, minX_B);
    for(int i = min(min_max, max_min); i <= max(min_max, max_min); ++i)
    {
        check = true;
        for(int j = min(A.y, B.y) + 1; j <= max(A.y, B.y) - 1; ++j)
        {
            if(arr[i][j] != 0) check = false;
        }
        if(check)
        {
            st_A.push_back({{i, A.y}, {i, B.y}});
        }
    }
    return st_A;
}

vector<pair<Point, Point>> checkY(Point A, Point B)
{
    vector<pair<Point, Point>> st_A;
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
    //bool check;
    int min_max = min(maxY_A, maxY_B);
    int max_min = max(minY_A, minY_B);
    for(int i = min(min_max, max_min); i <= max(min_max, max_min); ++i)
    {
        check = true;
        for(int j = min(A.x, B.x) + 1; j <= max(A.x, B.x) - 1; ++j)
        {
            if(arr[j][i] != 0) check = false;
        }
        if(check)
        {
            st_A.push_back({{A.x, i}, {B.x, i}});
        }
    }
    return st_A;
}

pair<Point, Point> road(Point A, Point B)
{
    vector<pair<Point, Point>> s1 = checkX(A, B);
    vector<pair<Point, Point>> s2 = checkY(A, B);
    pair<Point, Point> d = {A, B};
    int sum = 100;
    for(auto c : s2) s1.push_back(c);
    for(auto c : s1)
    {
        Point A1 = c.first;
        Point B1 = c.second;
        int ans = abs(A.x - A1.x + A.y - A1.y) + abs(A1.x - B1.x + A1.y - B1.y) + abs(B1.x - B.x + B1.y - B.y) + 1;
        if(ans < sum || (ans == sum && A1.x == A.x && A1.y == A.y) || (ans == sum && B.x == B1.x && B.y == B1.y))
        {
            sum = ans;
            d = {A1, B1};
        }
    }
    return d;
}

bool check_Exit_Pair()
{
    vector<Point> point;
    for(int i = 1; i <= column - 2; i++)
    {
        for(int j = 1; j <= row - 2; j++)
        {
            if(arr[i][j] > 0) point.push_back({i, j});
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

//kiểm tra bảng đã hết hay chưa
bool isGameOver()
{
    for(int i = 0; i < column; ++i)
    {
        for(int j = 0; j < row; ++j)
        {
            if(arr[i][j] > 0) return false;
        }
    }
    return true;
}

void load_time(int time)
{
    //SDL_RenderClear(renderer);
    SDL_Rect rect = {850, 600, 150, 90};
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderFillRect(renderer, &rect);
//  renderTexture("photos/bgr_score.jpg", 850, 600, 150, 90);
    drawCircle(renderer, 930, 645, 45);
    drawCircle(renderer, 930, 645, 44);
    drawCircle(renderer, 930, 645, 43);
    drawCircle(renderer, 930, 645, 42);
    loadFont(to_string(time), 900, 600, 60, 90, 2);
}


//trả về địa chỉ của con trỏ vừa thực hiện
Point mouse()
{
    SDL_Event e;
    bool kt = true;
    int x = -1, y = -1;
    time_t nowtime = time(NULL);
    //time_pause = time_lv;
    if(m[nowtime - time_end[level - 1]] == 0)
    {
        //time_pause = time_lv;
        //cout << level << " ";
        m[nowtime - time_end[level - 1]]++;
        if(time_lv - (nowtime - time_end[level - 1]) < 0)
        {
            lose = true;
            return {-1, -1};
        }
        load_time(time_lv - (nowtime - time_end[level - 1]));
        //load_time(time_pause);
        //time_pause--;
        //time_lv--;
        //cout << time_lv << " " << time_pause << endl;
        //cout << time_lv - (nowtime - time_end[level - 1]) << endl;
    }
    while(kt && SDL_PollEvent(&e))
    {
        if(e.type == SDL_QUIT)
        {
            check = false;
            SDL_Quit();
            break;
        }
        if(e.type == SDL_MOUSEBUTTONUP)
        {
            x = e.button.x;
            y = e.button.y;
            if(x >= 0 && y >= 0 && y <= SCREEN_HEIGHT && x <= SCREEN_WIDTH)
            {
                if(sqrt((235 - x) * (235 - x) + (635 - y) * (635 - y)) <= 35)
                {
                    return {11, 4};
                    kt = false;
                    break;
                }
                if(sqrt((975 - x) * (975 - x) + (50 - y) * (50 - y)) <= 35)
                {
                    return{0, 20};
                    kt = false;
                    break;
                }
                if(sqrt((1050 - x) * (1050 - x) + (50 - y) * (50 - y)) <= 25)
                {
                    return{0, 19};
                    kt = false;
                    break;
                }
                if(sqrt((900 - x) * (900 - x) + (50 - y) * (50 - y)) <= 25)
                {
                    return {0, 18};
                    kt = false;
                    break;
                }
                x -= const_column;
                x = (x + h_w - 1) / h_w;
                y -= const_row;
                y = (y + h_w - 1) / h_w;
                if(x >= 1 && y >= 1 && y <= column - 2 && x <= row - 2 && arr[y][x] > 0)
                {
                    return {y, x};
                    kt = false;
                    break;
                }
            }
        }
    }
    return {-1, -1};
}

int Ox(int n)
{
    n = (n - 1) * h_w + const_row + h_w / 2;
    return n;
}

int Oy(int n)
{
    n = (n - 1) * h_w + const_column + h_w / 2;
    return n;
}



void print_Road(Point A, Point B, int n)
{
    pair<Point, Point> p = road(A, B);
    Point B1 = p.second;
    Point A1 = p.first;
    SDL_SetRenderDrawColor(renderer, 16, 211, 204, 2);
    SDL_RenderDrawLine(renderer, Oy(A.y), Ox(A.x), Oy(A1.y), Ox(A1.x));
    SDL_RenderDrawLine(renderer, Oy(A1.y), Ox(A1.x), Oy(B1.y), Ox(B1.x));
    SDL_RenderDrawLine(renderer, Oy(B.y), Ox(B.x), Oy(B1.y), Ox(B1.x));

    SDL_RenderDrawLine(renderer, Oy(A.y) + 1, Ox(A.x) + 1, Oy(A1.y) + 1, Ox(A1.x) + 1);
    SDL_RenderDrawLine(renderer, Oy(A1.y) + 1, Ox(A1.x) + 1, Oy(B1.y) + 1, Ox(B1.x) + 1);
    SDL_RenderDrawLine(renderer, Oy(B.y) + 1, Ox(B.x) + 1, Oy(B1.y) + 1, Ox(B1.x) + 1);

    SDL_RenderDrawLine(renderer, Oy(A.y) - 1, Ox(A.x) - 1, Oy(A1.y) - 1, Ox(A1.x) - 1);
    SDL_RenderDrawLine(renderer, Oy(A1.y) - 1, Ox(A1.x) - 1, Oy(B1.y) - 1, Ox(B1.x) - 1);
    SDL_RenderDrawLine(renderer, Oy(B.y) - 1, Ox(B.x) - 1, Oy(B1.y) - 1, Ox(B1.x) - 1);
    SDL_RenderPresent(renderer);
    SDL_Delay(300 - n * 30);
}

bool mouse_random(Point A)
{
    if(A.x == 11 && A.y == 4) return true;
    return false;
}

bool mouse_sound(Point A)
{
    if(A.x == 0 && A.y == 18) return true;
    return false;
}

bool mouse_reset(Point A)
{
    if(A.x == 0 && A.y == 19) return true;
    return false;
}

bool mouse_main(Point A)
{
    if(A.x == 0 && A.y == 20) return true;
    return false;
}
//dùng để in hình ảnh background tại vị trí thay đổi

void printRect(Point A)
{
    if(arr[A.x][A.y] > 0) return;
    SDL_Rect rect;
    rect.w = h_w;
    rect.h = h_w;
    rect.x = (A.y - 1) * h_w + const_column;
    rect.y = (A.x - 1) * h_w + const_row;
//  string s = "photos/" + to_string(A.x) + "." + to_string(A.y) + ".png";
//  SDL_Texture *abc = loadTexture(s, renderer);
//  SDL_RenderCopy(renderer, abc, NULL, &rect);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderFillRect(renderer, &rect);
    //SDL_DestroyTexture(abc);
}

void print_random()
{
    for(int j = 1; j <= column - 2; ++ j)
    {
        for(int i = 1; i <= row - 2; ++i)
        {
            SDL_Rect rect;
            rect.w = h_w;
            rect.h = h_w;
            rect.x = (i - 1) * h_w + const_column;
            rect.y = (j - 1) * h_w + const_row;

            if(arr[j][i] > 0)
            {
                string s = "1/" + to_string(arr[j][i]) + "_down.png";
                SDL_Texture *abc = loadTexture(s, renderer);
                SDL_RenderCopy(renderer, abc, NULL, &rect);
                SDL_DestroyTexture(abc);
            }
            else
            {
                printRect({j, i});
            }
        }
    }
    SDL_RenderPresent(renderer);
}

void load_Box(int x, int y, int w, int h)
{
    SDL_Rect rect = {x, y, w, h};
    SDL_SetRenderDrawColor(renderer, 0, 0, 0,0);
    SDL_RenderFillRect(renderer, &rect);
}

void load_score()
{
    load_Box(950, 150, 130, 60);
    loadFont(to_string(score), 965, 150, 60, 60, 1);
}

void load_turn()
{
    load_Box(975, 310, 130, 60);
    loadFont(to_string(turn), 975, 310, 45, 60, 1);
}

void load_level()
{
    load_Box(975, 470, 130, 60);
    loadFont(to_string(level), 975, 470, 45, 60, 1);
}

load_end()
{
    loadFont("SCORE", 470, 470, 130, 50, 0);
    loadFont(to_string(score), 510, 520, 60, 60, 1);
}

//in map tại thời điểm hiên tại
void printMap()
{
    SDL_RenderClear(renderer);
    background("photos/youwin.png");
    SDL_Rect rect1 = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderFillRect(renderer, &rect1);
    SDL_Rect rect;
    rect.w = h_w;
    rect.h = h_w;

    for(int j = 1; j <= column - 2; ++ j)
    {
        for(int i = 1; i <= row - 2; ++i)
        {
            rect.x = (i - 1) * h_w + const_column;
            rect.y = (j - 1) * h_w + const_row;

            if(arr[j][i] > 0)
            {
                string s = "1/" + to_string(arr[j][i]) + "_down.png";
                SDL_Texture *abc = loadTexture(s, renderer);
                SDL_RenderCopy(renderer, abc, NULL, &rect);
                SDL_DestroyTexture(abc);
            }
        }
    }
    SDL_RenderPresent(renderer);
    rect1 = {200, 600, 70, 70};

//  SDL_SetRenderDrawColor(renderer, 23, 203, 83, 0);
//  SDL_RenderFillRect(renderer, &rect1);
    SDL_Texture *abc = loadTexture("photos/change.png", renderer);
    SDL_RenderCopy(renderer, abc, NULL, &rect1);
    SDL_DestroyTexture(abc);

    loadFont("SCORE", 930, 100, 130, 50, 0);
    loadFont("CHANGE TURN", 930, 260, 130, 50, 0);
    loadFont("LEVEL", 930, 420, 130, 50, 0);
    load_score();
    load_turn();
    load_level();
    reset_img();
    main_img();
    sound_img();
    //load_time(time_lv);

    SDL_RenderPresent(renderer);
    //SDL_Delay(100);
}

void lv(Point A, Point B, int n)
{
    switch(n)
    {
    case 1:
        break;

    case 2:
        if(A.y == B.y)
        {
            vector<int> v;
            for(int i = 1; i <= column - 2; ++i)
            {
                if(arr[i][A.y] > 0)
                {
                    v.push_back(arr[i][A.y]);
                    arr[i][A.y] = 0;
                }
            }
            for(int i = 1; i <= v.size(); ++i) arr[i][A.y] = v[i - 1];
        }
        else //if(A.x == B.x)
        {
            vector<int> v1, v2;
            for(int i = 1; i <= column - 2; ++i)
            {
                if(arr[i][A.y] > 0)
                {
                    v1.push_back(arr[i][A.y]);
                    arr[i][A.y] = 0;
                }
                if(arr[i][B.y] > 0)
                {
                    v2.push_back(arr[i][B.y]);
                    arr[i][B.y] = 0;
                }
            }
            for(int i = 1; i <= v1.size(); ++i) arr[i][A.y] = v1[i - 1];
            for(int i = 1; i <= v2.size(); ++i) arr[i][B.y] = v2[i - 1];
        }
        print_random();
        break;

    case 3:
        if(A.y == B.y)
        {
            vector<int> v;
            for(int i = 1; i <= column - 2; ++i)
            {
                if(arr[i][A.y] > 0)
                {
                    v.push_back(arr[i][A.y]);
                    arr[i][A.y] = 0;
                }
            }
            for(int i = 1; i <= v.size(); ++i) arr[column - 2 - v.size() + i][A.y] = v[i - 1];
        }
        else //if(A.x == B.x)
        {
            vector<int> v1, v2;
            for(int i = 1; i <= column - 2; ++i)
            {
                if(arr[i][A.y] > 0)
                {
                    v1.push_back(arr[i][A.y]);
                    arr[i][A.y] = 0;
                }
                if(arr[i][B.y] > 0)
                {
                    v2.push_back(arr[i][B.y]);
                    arr[i][B.y] = 0;
                }
            }
            for(int i = 1; i <= v1.size(); ++i) arr[column - 2 - v1.size() + i][A.y] = v1[i - 1];
            for(int i = 1; i <= v2.size(); ++i) arr[column - 2 - v2.size() + i][B.y] = v2[i - 1];
        }
        print_random();
        break;

    case 4:
        if(A.x == B.x)
        {
            vector<int> v;
            for(int i = 1; i <= row - 2; ++i)
            {
                if(arr[A.x][i] > 0)
                {
                    v.push_back(arr[A.x][i]);
                    arr[A.x][i] = 0;
                }
            }
            for(int i = 1; i <= v.size(); ++i) arr[A.x][i] = v[i - 1];
        }
        else //if(A.x == B.x)
        {
            vector<int> v1, v2;
            for(int i = 1; i <= row - 2; ++i)
            {
                if(arr[A.x][i] > 0)
                {
                    v1.push_back(arr[A.x][i]);
                    arr[A.x][i] = 0;
                }
                if(arr[B.x][i] > 0)
                {
                    v2.push_back(arr[B.x][i]);
                    arr[B.x][i] = 0;
                }
            }
            for(int i = 1; i <= v1.size(); ++i) arr[A.x][i] = v1[i - 1];
            for(int i = 1; i <= v2.size(); ++i) arr[B.x][i] = v2[i - 1];
        }
        print_random();
        break;

    case 5:
        if(A.x == B.x)
        {
            vector<int> v;
            for(int i = 1; i <= row - 2; ++i)
            {
                if(arr[A.x][i] > 0)
                {
                    v.push_back(arr[A.x][i]);
                    arr[A.x][i] = 0;
                }
            }
            for(int i = 1; i <= v.size(); ++i) arr[A.x][row - 2 - v.size() + i] = v[i - 1];
        }
        else //if(A.x == B.x)
        {
            vector<int> v1, v2;
            for(int i = 1; i <= row - 2; ++i)
            {
                if(arr[A.x][i] > 0)
                {
                    v1.push_back(arr[A.x][i]);
                    arr[A.x][i] = 0;
                }
                if(arr[B.x][i] > 0)
                {
                    v2.push_back(arr[B.x][i]);
                    arr[B.x][i] = 0;
                }
            }
            for(int i = 1; i <= v1.size(); ++i) arr[A.x][row - 2 - v1.size() + i] = v1[i - 1];
            for(int i = 1; i <= v2.size(); ++i) arr[B.x][row - 2 - v2.size() + i] = v2[i - 1];
        }
        print_random();
        break;

    case 6:
        if(A.y <= (row - 2) / 2)
        {
            vector<int> v;
            for(int i = 1; i <= (row - 2) / 2; ++i)
            {
                if(arr[A.x][i] > 0)
                {
                    v.push_back(arr[A.x][i]);
                    arr[A.x][i] = 0;
                }
            }
            for(int i = 1; i <= v.size(); ++i) arr[A.x][(row - 2) / 2 - v.size() +i] = v[i - 1];
        }
        else
        {
            vector<int> v;
            for(int i = (row - 2) / 2 + 1; i <= (row - 2); ++i)
            {
                if(arr[A.x][i] > 0)
                {
                    v.push_back(arr[A.x][i]);
                    arr[A.x][i] = 0;
                }
            }
            for(int i = 1; i <= v.size(); ++i) arr[A.x][(row - 2) / 2 + i] = v[i - 1];
        }

        if(B.y <= (row - 2) / 2)
        {
            vector<int> v;
            for(int i = 1; i <= (row - 2) / 2; ++i)
            {
                if(arr[B.x][i] > 0)
                {
                    v.push_back(arr[B.x][i]);
                    arr[B.x][i] = 0;
                }
            }
            for(int i = 1; i <= v.size(); ++i) arr[B.x][(row - 2) / 2 - v.size() +i] = v[i - 1];
        }
        else
        {
            vector<int> v;
            for(int i = (row - 2) / 2 + 1; i <= (row - 2); ++i)
            {
                if(arr[B.x][i] > 0)
                {
                    v.push_back(arr[B.x][i]);
                    arr[B.x][i] = 0;
                }
            }
            for(int i = 1; i <= v.size(); ++i) arr[B.x][(row - 2) / 2 + i] = v[i - 1];
        }
        print_random();
        break;

    case 7:
        if(A.y <= (row - 2) / 2)
        {
            vector<int> v;
            for(int i = 1; i <= (row - 2) / 2; ++i)
            {
                if(arr[A.x][i] > 0)
                {
                    v.push_back(arr[A.x][i]);
                    arr[A.x][i] = 0;
                }
            }
            for(int i = 1; i <= v.size(); ++i) arr[A.x][i] = v[i - 1];
        }
        else
        {
            vector<int> v;
            for(int i = (row - 2) / 2 + 1; i <= (row - 2); ++i)
            {
                if(arr[A.x][i] > 0)
                {
                    v.push_back(arr[A.x][i]);
                    arr[A.x][i] = 0;
                }
            }
            for(int i = 1; i <= v.size(); ++i) arr[A.x][row - 2 - v.size() + i] = v[i - 1];
        }

        if(B.y <= (row - 2) / 2)
        {
            vector<int> v;
            for(int i = 1; i <= (row - 2) / 2; ++i)
            {
                if(arr[B.x][i] > 0)
                {
                    v.push_back(arr[B.x][i]);
                    arr[B.x][i] = 0;
                }
            }
            for(int i = 1; i <= v.size(); ++i) arr[B.x][i] = v[i - 1];
        }
        else
        {
            vector<int> v;
            for(int i = (row - 2) / 2 + 1; i <= (row - 2); ++i)
            {
                if(arr[B.x][i] > 0)
                {
                    v.push_back(arr[B.x][i]);
                    arr[B.x][i] = 0;
                }
            }
            for(int i = 1; i <= v.size(); ++i) arr[B.x][row - 2 - v.size() + i] = v[i - 1];
        }
        print_random();
        break;

    case 8:
        if(A.x <= (column - 2) / 2)
        {
            vector<int> v;
            for(int i = 1; i <= (column - 2) / 2; ++i)
            {
                if(arr[i][A.y] > 0)
                {
                    v.push_back(arr[i][A.y]);
                    arr[i][A.y] = 0;
                }
            }
            for(int i = 1; i <= v.size(); ++i) arr[i][A.y] = v[i - 1];
        }
        else
        {
            vector<int> v;
            for(int i = (column - 2) / 2 + 1; i <= (column - 2); ++i)
            {
                if(arr[i][A.y] > 0)
                {
                    v.push_back(arr[i][A.y]);
                    arr[i][A.y] = 0;
                }
            }
            for(int i = 1; i <= v.size(); ++i) arr[column - 2 - v.size() + i][A.y] = v[i - 1];
        }

        if(B.x <= (column - 2) / 2)
        {
            vector<int> v;
            for(int i = 1; i <= (column - 2) / 2; ++i)
            {
                if(arr[i][B.y] > 0)
                {
                    v.push_back(arr[i][B.y]);
                    arr[i][B.y] = 0;
                }
            }
            for(int i = 1; i <= v.size(); ++i) arr[i][B.y] = v[i - 1];
        }
        else
        {
            vector<int> v;
            for(int i = (column - 2) / 2 + 1; i <= (column - 2); ++i)
            {
                if(arr[i][B.y] > 0)
                {
                    v.push_back(arr[i][B.y]);
                    arr[i][B.y] = 0;
                }
            }
            for(int i = 1; i <= v.size(); ++i) arr[column - 2 - v.size() + i][B.y] = v[i - 1];
        }
        print_random();
        break;

    case 9:
        if(A.x <= (column - 2) / 2)
        {
            vector<int> v;
            for(int i = 1; i <= (column - 2) / 2; ++i)
            {
                if(arr[i][A.y] > 0)
                {
                    v.push_back(arr[i][A.y]);
                    arr[i][A.y] = 0;
                }
            }
            for(int i = 1; i <= v.size(); ++i) arr[(column - 2) / 2 - v.size() + i][A.y] = v[i - 1];
        }
        else
        {
            vector<int> v;
            for(int i = (column - 2) / 2 + 1; i <= (column - 2); ++i)
            {
                if(arr[i][A.y] > 0)
                {
                    v.push_back(arr[i][A.y]);
                    arr[i][A.y] = 0;
                }
            }
            for(int i = 1; i <= v.size(); ++i) arr[(column - 2) / 2 + i][A.y] = v[i - 1];
        }

        if(B.x <= (column - 2) / 2)
        {
            vector<int> v;
            for(int i = 1; i <= (column - 2) / 2; ++i)
            {
                if(arr[i][B.y] > 0)
                {
                    v.push_back(arr[i][B.y]);
                    arr[i][B.y] = 0;
                }
            }
            for(int i = 1; i <= v.size(); ++i) arr[(column - 2) / 2 - v.size() + i][B.y] = v[i - 1];
        }
        else
        {
            vector<int> v;
            for(int i = (column - 2) / 2 + 1; i <= (column - 2); ++i)
            {
                if(arr[i][B.y] > 0)
                {
                    v.push_back(arr[i][B.y]);
                    arr[i][B.y] = 0;
                }
            }
            for(int i = 1; i <= v.size(); ++i) arr[(column - 2) / 2 + i][B.y] = v[i - 1];
        }
        print_random();
        break;

    default:
        break;
    }
}

void printTurn(Point A, int n)
{
    if(arr[A.x][A.y] == 0) return;
    SDL_Rect rect;
    string s;
    rect.w = h_w;
    rect.h = h_w;
    rect.x = (A.y - 1) * h_w + const_column;
    rect.y = (A.x - 1) * h_w + const_row;

    if(n == 0)
    {
        s = "1/" + to_string(arr[A.x][A.y]) + "_down.png";
    }
    else
    {
        s = "1/" + to_string(arr[A.x][A.y]) + "_up.png";
    }
    SDL_Texture *abc = loadTexture(s, renderer);
    SDL_RenderCopy(renderer, abc, NULL, &rect);
    SDL_RenderPresent(renderer);
    SDL_DestroyTexture(abc);
}

// chỉ in vị trí có thay đổi
void printPoint(Point A, Point B)
{
    pair<Point, Point> p = road(A, B);
    arr[A.x][A.y] = 0;
    arr[B.x][B.y] = 0;
    Point B1 = p.second;
    Point A1 = p.first;
    if(A.x == A1.x)
    {
        for(int i = min(A1.y, A.y); i <= max(A.y, A1.y); ++i) printRect({A.x, i});
    }
    else //if(A.y == A1.y)
    {
        for(int i = min(A1.x, A.x); i <= max(A.x, A1.x); ++i) printRect({i, A.y});
    }

    if(B1.x == A1.x)
    {
        for(int i = min(B1.y, A1.y); i <= max(B1.y, A1.y); ++i) printRect({A1.x, i});
    }
    else //if(B1.y == A1.y)
    {
        for(int i = min(A1.x, B1.x); i <= max(B1.x, A1.x); ++i) printRect({i, A1.y});
    }

    if(B.x == B1.x)
    {
        for(int i = min(B1.y, B.y); i <= max(B1.y, B.y); ++i) printRect({B.x, i});
    }
    else //if(B.y == B1.y)
    {
        for(int i = min(B1.x, B.x); i <= max(B.x, B1.x); ++i) printRect({i, B.y});
    }
}

void printBoard()
{
    for(int i = 0; i < column; ++i)
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

void menu();

void reset()
{
    Mix_HaltMusic();
    if(sound == 0) Mix_PlayMusic( gMusic, -1 );
    SDL_RenderClear(renderer);
    makeArr();
    score = 0;
    turn = 10;
    level = 1;
    time_lv = lv_time + 20 * (level - 1);
    for(int i = 1; i < 100; ++i) time_end[i] = 0;
    time_end[0] = time(NULL);
    m.clear();
    menu();
    lose = false;
    check = true;
}

void lose_game()
{
    Mix_HaltMusic();
    background("photos/lose.gif");
    load_end();
    //return;
}

void win_game()
{
    Mix_HaltMusic();
    background("photos/won.jpg");
    load_end();
}

void start();
void level_();

void print_Rank()
{
    SDL_SetRenderDrawColor(renderer, 225, 125, 180, 0);
    SDL_Rect rect = {150, 300, 800, 300};
    SDL_Texture *score = loadTexture("photos/scoreBoard.png", renderer);
    SDL_RenderCopy(renderer, score, NULL, &rect);
    SDL_RenderPresent(renderer);
    SDL_DestroyTexture(score);

    loadFont("Top 1 : " + to_string(score_high[0]), 210, 330, 180, 70, 2);
    loadFont("Top 2 : " + to_string(score_high[1]), 210, 410, 180, 70, 2);
    loadFont("Top 3 : " + to_string(score_high[2]), 210, 490, 180, 70, 2);
    loadFont("Top 4 : " + to_string(score_high[3]), 465, 300, 180, 70, 2);
    loadFont("Top 5 : " + to_string(score_high[4]), 465, 370, 180, 70, 2);
    loadFont("Top 6 : " + to_string(score_high[5]), 465, 450, 180, 70, 2);
    loadFont("Top 7 : " + to_string(score_high[6]), 465, 530, 180, 70, 2);
    loadFont("Top 8 : " + to_string(score_high[7]), 720, 330, 180, 70, 2);
    loadFont("Top 9 : " + to_string(score_high[8]), 720, 410, 180, 70, 2);
    loadFont("Top 10 : " + to_string(score_high[9]), 720, 490, 180, 70, 2);
}

void high_score()
{
    ifstream file("highscore.txt");
    for(int i = 0; i < 10; ++i)
    {
        file >> score_high[i];
    }
    print_Rank();

    SDL_Texture *back = loadTexture("photos/back.png", renderer);

    SDL_Rect rect1 = {105, 90, 100, 40};
    SDL_Rect rect2 = {80, 80, 150, 60};

    SDL_Rect rect = {0, 0, 250, 250};
    SDL_Texture *sky= loadTexture("photos/sky.png", renderer);

    bool running = true;
    bool zoom_back = false;
    SDL_Event e;
    while(running)
    {
        while(SDL_WaitEvent(&e))
        {
            bool pass = false;
            if(e.type == SDL_QUIT)
            {
                SDL_Quit();
                running = false;
                break;
            }
            if(e.type == SDL_MOUSEMOTION)
            {
                int x = e.motion.x;
                int y = e.motion.y;
                SDL_RenderCopy(renderer, sky, NULL, &rect);

                if(x >= rect1.x && y >= rect1.y && y <= rect1.y + rect1.h && x <= rect1.x + rect1.w)
                {
                    SDL_RenderCopy(renderer, back, NULL, &rect2);
                    zoom_back = true;
                }
                else
                {
                    zoom_back = false;
                    SDL_RenderCopy(renderer, back, NULL, &rect1);
                }
                SDL_RenderPresent(renderer);
            }
            if(e.type == SDL_MOUSEBUTTONUP)
            {
                int x = e.button.x;
                int y = e.button.y;
                if(x >= rect1.x && y >= rect1.y && y <= rect1.y + rect1.h && x <= rect1.x + rect1.w)
                {
                    start();
                    //check = true;
                    running = false;
                    pass = true;
                }
            }
            if(pass) break;
        }
    }
    SDL_DestroyTexture(back);
}

void pushScore()
{
    ofstream file("highscore.txt");
    score_high[10] = score;
    sort(score_high, score_high + 11);
    reverse(score_high, score_high + 11);
    score_high[10] = 0;
    for(int i = 0; i < 10; ++i) file << score_high[i] << endl;
}

void menu()
{
    //cout << time_pause << endl;
    //time_lv = time_pause;
}

//chạy chương trình
void level_()
{
    printMap();
    time_end[level - 1] = time(NULL);
    do
    {
        bool pass = false;
        if(turn == 0 && !check_Exit_Pair())
        {
            lose = true;
            //lose_game();
            break;
        }

        if(!check_Exit_Pair() && turn > 0)
        {
            turn--;
            random();
            print_random();
            load_turn();
            continue;
        }
        if(lose) break;
        Point turn1 = mouse();
        while(turn1.x == -1 || turn1.y == -1)
        {
            turn1 = mouse();
            if(lose) break;
            while(mouse_sound(turn1))
            {
                mix();
                turn1 = mouse();
            }
            while(mouse_main(turn1))
            {
                sound = 1;
                //menu();
                reset();
                //time_lv = lv_time + 20 * (level - 1);
                start();
                //sMenu();
                pass = true;

                continue;
            }
            while(turn == 0 && mouse_random(turn1)) turn1 = mouse();
        }
        if(pass) break;

        if(mouse_random(turn1) && turn > 0)
        {
            turn--;
            random();
            print_random();
            load_turn();
            continue;
        }
        else if(mouse_reset(turn1))
        {
            reset();
            level_();
            break;
        }
        else if(check) printTurn(turn1, 1);
        loadTurn();
        if(lose) break;

        Point turn2 = mouse();
        while(turn2.x == -1 || turn2.y == -1)
        {
            turn2 = mouse();
            if(lose) break;

            while(mouse_sound(turn2))
            {
                mix();
                turn2 = mouse();
            }
            while(mouse_main(turn2))
            {
                reset();
                start();
                //sMenu();
                pass = true;
                continue;
            }
            while(turn == 0 && mouse_random(turn2)) turn2 = mouse();
        }
        if(pass) break;

        if(mouse_random(turn2) && turn > 0)
        {
            turn--;
            random();
            print_random();
            load_turn();
            continue;
        }
        else if(mouse_reset(turn2))
        {
            reset();
            level_();
            break;
        }
        else if(check) printTurn(turn2, 1);
        //loadTurn();
        if(lose) break;

        if(checkX(turn1, turn2).size() != 0 || checkY(turn1, turn2).size() != 0)
        {
            loadDelete();
            print_Road(turn1, turn2, level);
            printPoint(turn1, turn2);
            lv(turn1, turn2, level);
            score += 10 + (level - 1) * 2;
            load_score();
        }
        else if(check)
        {
            loadNoDelete();
            printTurn(turn1, 0);
            printTurn(turn2, 0);
        }
        if(isGameOver())
        {
            time_end[level] = time(NULL);
            score += (time_end[level] - time_end[level - 1]) * (7 + (level - 1) * 2) / 10;
            if(level < levelMax)
            {
                SDL_RenderClear(renderer);
                m.clear();
                makeArr();
                level++;
                turn = min(10, turn + 1);
                time_lv = lv_time + 20 * (level - 1);
                printMap();
            }
            else
            {
                pushScore();
                win_game();
                //lose = true;
                break;
            }
        }
    }
    while(!isGameOver() && !lose);
    if(lose)
    {
        pushScore();
        lose_game();
    }
}


void play_game()
{
    sound = 0;
    loadMedia();
    level_();
}

//màn hình menu vào game
void start()
{
    Uint32 frameStart;
    int frameTime;

    SDL_RenderClear(renderer);

    SDL_Texture *background = loadTexture("photos/youlose.png", renderer);
    SDL_Texture *start = loadTexture("photos/play.png", renderer);
    SDL_Texture *intro = loadTexture("photos/intro.png", renderer);
    SDL_Texture *score = loadTexture("photos/scr.png", renderer);

    SDL_Rect rect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

    SDL_Rect rect1 = {410, 382, 280, 96};
    SDL_Rect rect2 = {400, 370, 300, 120};

    SDL_Rect rect3 = {410, 502, 280, 96};
    SDL_Rect rect4 = {400, 470, 300, 120};

    SDL_Rect rect5 = {410, 622, 280, 96};
    SDL_Rect rect6 = {400, 570, 300, 120};
    //SDL_RenderCopy(renderer, start, NULL, &rect1)
    bool running = true;
    bool exit = false;
    SDL_Event e;
    while(running)
    {
        frameStart = SDL_GetTicks();
        while(SDL_WaitEvent(&e))
        {
            if(e.type == SDL_QUIT)
            {
                SDL_Quit();
                running = false;
                continue;
            }
            if(e.type == SDL_MOUSEMOTION)
            {
                int x = e.motion.x;
                int y = e.motion.y;
                SDL_RenderClear(renderer);
                SDL_RenderCopy(renderer, background, NULL, &rect);

                if(x >= rect1.x && y >= rect1.y && y <= rect1.y + rect1.h && x <= rect1.x + rect1.w)
                {
                    SDL_RenderCopy(renderer, start, NULL, &rect2);
                }
                else
                {
                    SDL_RenderCopy(renderer, start, NULL, &rect1);
                }

                if(x >= rect3.x && y >= rect3.y && y <= rect3.y + rect3.h && x <= rect3.x + rect3.w)
                {
                    SDL_RenderCopy(renderer, score, NULL, &rect4);
                }
                else
                {
                    SDL_RenderCopy(renderer, score, NULL, &rect3);
                }

                if(x >= rect5.x && y >= rect5.y && y <= rect5.y + rect5.h && x <= rect5.x + rect5.w)
                {
                    SDL_RenderCopy(renderer, intro, NULL, &rect6);
                }
                else
                {
                    SDL_RenderCopy(renderer, intro, NULL, &rect5);
                }
                SDL_RenderPresent(renderer);
            }
            if(e.type == SDL_MOUSEBUTTONUP)
            {
                int x = e.button.x;
                int y = e.button.y;
                if(x >= rect1.x && y >= rect1.y && y <= rect1.y + rect1.h && x <= rect1.x + rect1.w)
                {
                    play_game();
                    exit = true;
                    running = false;
                    continue;
                }

                if(x >= rect3.x && y >= rect3.y && y <= rect3.y + rect3.h && x <= rect3.x + rect3.w)
                {
                    high_score();
                    //waitUntilKeyPressed();
                    exit = true;
                    running = false;
                    //break;
                    continue;
                }
            }
            if(exit) break;
        }

        frameTime = SDL_GetTicks()- frameStart;

        if(FRAME_DELAY > frameTime)
        {
            SDL_Delay(FRAME_DELAY - frameTime);
        }
    }
    SDL_RenderPresent(renderer);
    SDL_DestroyTexture(background);
    SDL_DestroyTexture(start);
    SDL_DestroyTexture(intro);
    SDL_DestroyTexture(score);
}

int main(int argc, char* argv[])
{
    ios_base::sync_with_stdio(false);

    initSDL(window, renderer);
    SDL_RenderClear(renderer);

    makeArr();
    start();
    //printMap();

    SDL_RenderPresent(renderer);
    waitUntilKeyPressed();
    quitSDL(window, renderer);
    return 0;
}
