/*1. Используя функции SDL (SDL_RenderDraw... и SDL_RenderFill...) нарисовать:
 *         - поверхность земли
 *         - траву
 *         - дом с крышей, окном, дверью и трубой
 *         - на заднем плане - забор (опционально)
 * 2. Реализовать рисование окружности с использованием алгоритма Брезенхэма
 * 3. Нарисовать у домика круглое окно на чердаке.
 */
#include <iostream>
#include <memory>
/*
 * std::shared_ptr - разделяемый указатель
 * std::unique_ptr - уникальный указатель
 * std::weak_ptr - "слабый" указатель
 */
#include <SDL2/SDL_main.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
constexpr int WIDTH = 1000, HEIGHT = 720, R = 25;

std::shared_ptr<SDL_Texture> icon;
std::shared_ptr<SDL_Texture> icon2;

std::shared_ptr<SDL_Texture> Make_icon(SDL_Renderer *renderer, const char *name)
{

	std::shared_ptr<SDL_Texture> picture = std::shared_ptr<SDL_Texture>(
			IMG_LoadTexture(renderer, name),
			SDL_DestroyTexture);
	if(picture == nullptr)
	{
		std::cerr << "Не могу загрузить картинку" <<
		SDL_GetError()<<std::endl;
		return nullptr;
	}
	return picture;
}


void DrawCircle(SDL_Renderer * ren, int x, int y, int r, int X, int Y)
{
            int delta = 1 - 2 * r;
            int error = 0;
            SDL_SetRenderDrawColor(ren, 0, 190, 190, 1);

            while(y >= 0)
            {
                SDL_RenderDrawPoint(ren, X + x, Y + y);
                SDL_RenderDrawPoint(ren, X + x, Y - y);
                SDL_RenderDrawPoint(ren, X - x, Y + y);
                SDL_RenderDrawPoint(ren, X - x, Y - y);
                error = 2 * (delta + y) - 1;
                if (delta < 0 and error <= 0)
                {
                    ++x;
                    delta += 2 * x + 1;
                    continue;
                }
                if (delta > 0 and error > 0)
                {
                    --y;
                    delta -= 2 * y + 1;
                    continue;
                }
                ++x;
                --y;
                delta += 2 * (x - y);
            }
}
int main(int, char **)
{
    std::cout << "Начало работы программы" << std::endl;
    SDL_Init(SDL_INIT_EVERYTHING);
    auto win = std::shared_ptr<SDL_Window>(
            SDL_CreateWindow("Home",
                    SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                    WIDTH, HEIGHT,
                    SDL_WINDOW_SHOWN),
            SDL_DestroyWindow);
    if (win == nullptr) {
        std::cerr << "Ошибка при создании окна: " <<
                SDL_GetError() << std::endl;
        return 1;
    }
    auto ren = std::shared_ptr<SDL_Renderer>(
            SDL_CreateRenderer(win.get(), -1,
                    SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC),
            SDL_DestroyRenderer);
    if (ren == nullptr) {
        std::cerr << "Ошибка при создании рендерера: " <<
                SDL_GetError() << std::endl;
        return 1;
    }


	bool f = true;
	int icon_x = 640, icon_y = 90;// координаты где появиться летающий
	bool f2 = true;
	int icon_x2 = 500, icon_y2 = 500;// координаты где появится бегающий

	auto keys = SDL_GetKeyboardState(nullptr);

    SDL_Event event;
    bool finish = false;
    // Основной цикл программы
    while (not finish)
    {
        // Цикл обработки событий
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) // Если окно закрыли...
                finish = true;
        }
        //Фон
        SDL_SetRenderDrawColor(ren.get(), 175, 255, 255, 1);//Цвет в формате rgba
        SDL_RenderClear(ren.get());
        //Земля
        SDL_Rect r { 0, 525, WIDTH, 220 };
        SDL_SetRenderDrawColor(ren.get(), 0, 128, 0, 1);//Цвет в формате rgba
        SDL_RenderFillRect(ren.get(), &r);


        SDL_SetRenderDrawColor(ren.get(), 190, 98, 0, 1);
        SDL_Rect rr2 { 0, 425, WIDTH, 10 };
        SDL_RenderFillRect(ren.get(), &rr2);
        SDL_Rect rr3 { 0, 500, WIDTH, 10 };
        SDL_RenderFillRect(ren.get(), &rr3);

        //забор
        for (int i = 20; i < WIDTH; i += 50) {
        	SDL_SetRenderDrawColor(ren.get(), 255, 174, 0, 1);// палки на заборе
            SDL_Rect r1 { i, 400, 25, 150 };
            SDL_RenderFillRect(ren.get(), &r1);
            for (double x1 = i + 12.5, x2 = i + 12.5,  y = 375; y < 400; x1 -= 0.5 , x2 += 0.5 , y++)
            {
            	SDL_RenderDrawLine(ren.get(), x1, y, x2, y);
            }
        }
        //дом
        SDL_Rect r1 { 350, 300, 450, 300 };
        SDL_SetRenderDrawColor(ren.get(), 172, 163, 74, 1);//Цвет в формате rgba
        SDL_RenderFillRect(ren.get(), &r1);

        //Окно
        SDL_Rect r2 { 660, 400, 100, 100 };
        SDL_SetRenderDrawColor(ren.get(), 0, 190, 190, 1);//внутри //Цвет в формате rgba
        SDL_RenderFillRect(ren.get(), &r2);

        SDL_SetRenderDrawColor(ren.get(), 0, 0, 0, 1);// палочки //Цвет в формате rgba
        SDL_RenderDrawLine(ren.get(), 710, 400, 710, 500);

        SDL_SetRenderDrawColor(ren.get(), 0, 0, 0, 1);// палочки //Цвет в формате rgba
        SDL_RenderDrawLine(ren.get(), 660, 450, 760, 450);

        SDL_Rect w2 { 390, 400, 100, 100 };
        SDL_SetRenderDrawColor(ren.get(), 0, 190, 190, 1);//внутри //Цвет в формате rgba
        SDL_RenderFillRect(ren.get(), &w2);

        SDL_SetRenderDrawColor(ren.get(), 0, 0, 0, 1);// палочки //Цвет в формате rgba
        SDL_RenderDrawLine(ren.get(), 440, 400, 440, 500);

        SDL_SetRenderDrawColor(ren.get(), 0, 0, 0, 1);// палочки //Цвет в формате rgba
        SDL_RenderDrawLine(ren.get(), 390, 450, 490, 450);

        //Труба
        SDL_Rect r3 { WIDTH / 2 + 175, 130, 30, 150 };
        SDL_SetRenderDrawColor(ren.get(), 66, 45, 13, 1);//Цвет в формате rgba
        SDL_RenderFillRect(ren.get(), &r3);
        //Крыша
        SDL_SetRenderDrawColor(ren.get(), 100, 0, 0, 1);//Цвет в формате rgba
        for (double x1 = 575, x2 = 575,  y = 130; y < 300; x1 -= 1.5, x2 += 1.5, y++) {
            SDL_RenderDrawLine(ren.get(), x1, y, x2, y);
        }
        //дверь
        SDL_Rect r4 { 525, 400, 100, 200 };
        SDL_SetRenderDrawColor(ren.get(), 66, 45, 13, 1);//Цвет в формате rgba
        SDL_RenderFillRect(ren.get(), &r4);

        //окружность
        for (int r = 40; r > 0; r--){
        	DrawCircle(ren.get(), 0, r, r, 575, 225);
        }

		//Летающая птичка
		if (f)
			icon = Make_icon(ren.get(), "birdRight.png");
		else
			icon = Make_icon(ren.get(), "birdLeft.png");

		if(keys[SDL_SCANCODE_RIGHT] and icon_x < WIDTH - 100){ icon_x += 8; f = true;}//тут и далее цифра 8 это скорость полета
		if(keys[SDL_SCANCODE_LEFT] and icon_x > 0 ){ icon_x -= 8; f = false;}
		if(keys[SDL_SCANCODE_UP] and icon_y > 0){ icon_y -= 8; }
		if(keys[SDL_SCANCODE_DOWN] and icon_y < HEIGHT - 50){ icon_y += 8; }

		if(icon == nullptr) return 1;
		SDL_Rect icon_rect {icon_x, icon_y, 105, 100};//тут предпоследние 2 числа это  ширина и высота картинки твоей
		SDL_RenderCopy(ren.get(), icon.get(), nullptr, &icon_rect);



		//бегающий лев
		if(icon_x2 < WIDTH - 150 and f2){
			icon2 = Make_icon(ren.get(), "lionRight.png");
			icon_x2 += 4;//скорость бега вправо
		}
		else f2 = false;
		if(icon_x2 > 0 and !f2){
			icon2 = Make_icon(ren.get(), "lionLeft.png");
			icon_x2 -= 4;//скорость бега влево
		}
		else f2 = true;

		if(icon2 == nullptr) return 1;
		SDL_Rect icon_rect2 {icon_x2, icon_y2, 200, 200};//тут предпоследние 2 числа это  ширина и высота картинки твоей
		SDL_RenderCopy(ren.get(), icon2.get(), nullptr, &icon_rect2);


        SDL_RenderPresent(ren.get());
        }
        /*
         * Двойная буферизация:
         * 1 буфер (передний) - буфер, содержимое которого отображается.
         * 2 буфер (задний) - буфер, в котором осуществляется построение изображения.
         *
         */
        std::cout << "Конец работы программы" << std::endl;
        return 0;
}
