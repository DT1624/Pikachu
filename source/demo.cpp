#include <bits/stdc++.h>
using namespace std;

//hàm random
long long Rand(long long l, long long h)
{
    return l + ((long long)rand() * (RAND_MAX + 1) * (RAND_MAX + 1) * (RAND_MAX + 1) +
                (long long)rand() * (RAND_MAX + 1) * (RAND_MAX + 1) +
                (long long)rand() * (RAND_MAX + 1) +
                rand()) % (h - l + 1);
}

//long long Rand(int l, int h) {
//    return (long long)rand() % (h - l) + l;
//}

const double xsditruyen = 0.7;
const double xsbiendi = 0.05;
const double r = 50;//bán kính của trạm phát(tính theo dam - đề ca mét)
const int quanthe = 100;//số lượng quần thể ban đầu
int number = 100;//số lượt di truyền
int numberrandom = 500;
const int r_area1 = 1500000;//đơn vị cm
const int r_area = 200000;//đơn vị cm


vector<pair<double, double>> v;//tập quần thể
vector<pair<double, double>> house;//tập hộ gđ
vector<pair<double, double>> cathemax;//tập chứa các cá thể max ở từng vòng đời
vector<pair<double, double>> cathemin;

//tính độ thích nghi của từng cá thể {x, y}
int f(vector<pair<double, double>> house, pair<double, double> p) {
  int count = 0;
  for(auto c : house) {
    double x = p.first, y = p.second, z = c.first, t = c.second;
    x = abs(x - z);
    y = abs(y - t);
    if((x * x + y * y) <= r * r) ++count;
  }
  return count;
}

//hàm sort cho quần thể
double sort_v(pair<double, double> a, pair<double, double> b) {
  return f(house, a) < f(house, b);
}

//tìm cá thể có độ thích nghi lớn nhất.
pair<double, double> max_cathe(vector<pair<double, double>> house, vector<pair<double, double>> v) {
  int mx = 0;
  pair<double, double> ans = v[0];
  for(auto c : v) {
    if(mx < f(house, c)) {
      mx = f(house, c);
      ans = c;
    }
  }
  return ans;
}
//cá thể có độ thích nghi nhỏ nhất
pair<double, double> min_cathe(vector<pair<double, double>> house, vector<pair<double, double>> v) {
  int mn = 0;
  pair<double, double> ans = v[0];
  for(auto c : v) {
    if(mn > f(house, c)) {
      mn = f(house, c);
      ans = c;
    }
  }
  return ans;
}

//tạo mảng chứa xác suất tích lũy của quần thế
vector<double> xstichluy(vector<pair<double, double>> house, vector<pair<double, double>> v) {
  vector<double> ans;
  vector<double> abc;
  for(int i = 0; i < v.size(); ++i) {
    if(i == 0) abc.push_back(f(house, v[i]));
    else abc.push_back(abc[abc.size() - 1] + f(house, v[i]));
  }
  if(abc[abc.size() - 1] == 0) return {0};
  for(auto c : abc) {
    c = c / abc[abc.size() - 1];
    ans.push_back(c);
  }
  return ans;
}
//tạo các hộ gia đình
//void houses() {
//  //tọa độ điểm tập trung
//  ofstream file("DITRUYEN_test2.txt");
//  double arrx[5], arry[5];
//  for(int i = 0; i < 5; ++i) {
//    arrx[i] = Rand(1, r_area);
//    arry[i] = Rand(1, r_area);
//    arrx[i] /= 1000;
//    arry[i] /= 1000;
//      cout << arrx[i] << " " << arry[i] << endl;
//  }
//
//  int n = 27664;
//  file << n << endl;
//  //kéo các điểm gần lại các điểm tập trung
//  cout << endl << "Toa do cua cac ho gia dinh la:" << endl << endl;
//  for(int i = 0; i < n; ++i) {
//    double ratio = rand() % 48  + 1;
//    double x = Rand(1, r_area) + 1;
//    double y = Rand(1, r_area) + 1;
//    x /= 1000; y /= 1000;
//
//    x = x + (arrx[i % 5] - x) * ratio / 50;
//    y = y + (arry[i % 5] - y) * ratio / 50;
//    file << x << " " << y << endl;
//    house.push_back({x, y});
//  }
//}

void houses() {
  ifstream file("Ditruyen.txt");
  int n;
  double x, y;
  file >> n;
  while(n--) {
    file >> x >> y;
    house.push_back({x, y});
    cout << x << " " << y << endl;
  }
}

int main(int argc, const char * argv[]) {
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    srand((int)time(NULL));
    cout << setprecision(15) << fixed;

    houses();//tạo toạ độ các hộ gia đình



    cout << "Co " << house.size() << " ho gia dinh" << endl;
    cout << endl << "Quan the da duoc tao la:" << endl << endl;

    //tạo quần thể
    for(int i = 0; i < quanthe; ++i) {
    double x = Rand(1, r_area);
    double y = Rand(1, r_area);
    x /= 1000; y /= 1000;
    cout << x << " " << y << endl;
    v.push_back({x, y});
    }

    //lưu cá thể max ban đầu
    pair<double, double> kq_max = max_cathe(house, v), abc = max_cathe(house, v);
    pair<double, double> kq_min = min_cathe(house, v), def = min_cathe(house, v);
    cathemax.push_back(kq_max);
    cathemin.push_back(kq_min);

    //In ra độ thích nghi của các cá thể
    cout << endl << "Do thich nghi cua cac ca the la:" << endl << endl;
    for(auto c : v) {
    cout << f(house, c) << endl;
    }

    //In ra xác suất tích luỹ của quần thể ban đầu
    vector<double> s = xstichluy(house, v);
    cout << endl << "Xac suat tich luy cua cac ca the ban dau lan luot la:" << endl << endl;
    for(auto c : s) cout << c << endl;

    //Quá trình di truyền
    while(number--) {

        if (number == 20) {
            for (auto c : v) {
                cout << c.first << " " << c.second << endl;
            }
        }


        //sắp xếp quần thể
        sort(v.begin(), v.end(), sort_v);

        //bánh xe xổ số
        vector<double> q = xstichluy(house, v);
        vector<pair<double, double>> v1;
        for(int i = 0; i < v.size() / 2 ; ++i) {
            double x = (double)Rand(0, 10000) / 10000;
            int vitrimin = lower_bound(q.begin(), q.end(), x) - q.begin();
            v1.push_back({v[vitrimin].first, v[vitrimin].second});
        }
        for(int i = 0; i < v.size() / 2 ; ++i) {
            v[i] = v1[i];
        }

        //tương giao chéo
        for(int i = 0; i < v.size() / 2; i += 2) {
          double x = (double)Rand(0, 10000) / 10000;
          if(x < xsditruyen) {
            double alpha = (double)Rand(0, 1000) / 1000;
            double tmp1 = v[i].first, tmp2 = v[i].second;
            double tmp3 = v[i + 1].first, tmp4 = v[i + 1].second;

            v[i].first = alpha * tmp1 + (1 - alpha) * tmp3;
            v[i].second = alpha * tmp2 + (1 - alpha) * tmp4;
            v[i + 1].first = alpha * tmp3 + (1 - alpha) * tmp1;
            v[i + 1].second = alpha * tmp4 + (1 - alpha) * tmp2;
          }
        }

        //biến dị
        for(int i = 0; i < v.size(); i++) {
          double x = (double)Rand(0, 10000) / 10000;
          if(x < xsbiendi) {
            double y = (double)Rand(0, 10000) / 10000;
            if(y < xsbiendi) {
              v[i].first = (double)Rand(1, r_area) / 1000;
              v[i].second = (double)Rand(1, r_area) / 1000;
            } else {
              if(rand() % 2) {
                v[i].first = (double)Rand(1, r_area) / 1000;
              } else {
                v[i].second = (double)Rand(1, r_area) / 1000;
              }
            }
        //        c.first = (double)Rand(1, r_area) / 1000;
        //        c.second = (double)Rand(1, r_area) / 1000;
          }
        }
        kq_max = max_cathe(house, v);
        kq_min = min_cathe(house, v);
        cathemax.push_back(kq_max);
        cathemin.push_back(kq_min);
        if(f(house, kq_max) > f(house, abc)) abc = kq_max;
        if(f(house, kq_min) < f(house, def)) def = kq_min;
    }
    ofstream file_out("output_test2_1.txt");
    for(int i = 0; i < cathemax.size(); ++i) file_out << f(house, cathemax[i]) << " " << f(house, cathemin[i]) << endl;
    //In ra độ thích nghi lớn nhất mỗi vòng đời
    cout << "Do thich nghi lon nhat va nho nhat moi vong doi la" << endl;
    for(int i = 0; i < cathemax.size(); ++i) cout << f(house, cathemax[i]) << " " << f(house, cathemin[i]) << endl;

    //In ra vị trí tốt nhất để đặt trạm vào file Ditruyen.txt
    cout << "Mot trong cac vi tri tot nhat de dat tram phat song la" << endl;
    cout << abc.first << " " << abc.second << " " << f(house,abc) << endl;

    //In tập quần thể cuối
    for (auto c : v) {
        cout << c.first << " " << c.second << endl;
    }




//    ifstream file("Ditruyen.txt");
//    file << house.size() << endl;
//    for(auto c : house) {
//        file << c.first << " " << c.second << endl;
//    }
//    file << setprecision(2) << fixed << abcdef.first << " " << abcdef.second << " " << f(house, abcdef) << " end.";

    cout << "Thuat toan random tra ve ket qua la:" << endl;
    //Thuat toan random
    int maxrandom = 0;
    pair<double, double> maxpair;
    while (numberrandom--) {
        double x = Rand(1, r_area);
        double y = Rand(1, r_area);
        x/= 1000; y/=1000;
        pair<double, double> var = {x, y};
        if (maxrandom < f(house, var)) {
            maxrandom = f(house, var);
            maxpair = var;
        }
        cout << maxpair.first << " " << maxpair.second << " " <<  maxrandom << endl;
    }



  return 0;

}


