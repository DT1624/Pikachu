# -------->PIKACHU<--------

# Luật chơi như sau: 
  + Bạn cần tìm các cặp ô giống nhau sao cho tồn tại đường đi giữa chúng.
  + Đường đi giữa 2 ô được định nghĩa là đường nối mà chỉ có tối đa 2 lần rẽ (đường gấp khúc có tối đa 3 đoạn).
  + Khi bạn loại bỏ thành công 1 cặp ô, bạn sẽ nhận được điểm tương ứng level hiện tại.
  + Khi không tìm được đường đi bất kỳ và còn cơ hội(chance), game sẽ random ngẫu nhiên vị trí các ô. Ngược lại nếu đã hết cơ hội, bạn sẽ thua.
  + Bạn sẽ qua level nếu trên màn hình không còn ô nào, điểm sẽ được cộng dựa theo lượng thời gian còn lại và level hiện tại.
  + Sau khi vượt qua 9 level, bạn sẽ là người chiến thắng.
# Yêu cầu:
  + Code::Blocks // Microsoft Visual Studio
  + Thư viện SDL(SDL, SDL_image, SDL_ttf, SDL_mixer) 
  + Download in: + https://github.com/libsdl-org/SDL/releases/tag/release-2.26.4
                 / https://github.com/libsdl-org/SDL_image/releases
                 / https://github.com/libsdl-org/SDL_ttf/releases
                 / https://github.com/libsdl-org/SDL_mixer/releases
