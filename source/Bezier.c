#include <Bezier.h>

#include <SDL2/SDL.h>

SDL_Point P1 = {
    .x = 100,
    .y = 200
};

SDL_Point P2 = {
    .x = 200,
    .y = 0
};

SDL_Point P3 = {
    .x = 300,
    .y = 200
};

SDL_Window * WINDOW = NULL;
SDL_Renderer * RENDERER = NULL;

int is_mouse_button_down = 0;

int check_mouse_rect_collision(SDL_Point p_center, int sz) {
    int x, y;
    SDL_GetMouseState(&x, &y);

    int delta_x = x - p_center.x;
    int delta_y = y - p_center.y;

    float dp2 = delta_x * delta_x + delta_y * delta_y;

    return (SDL_sqrtf(dp2) <= sz);
}

static void bezier();

void bz_initiate(const char * title, int width, int height)
{
    SDL_Init(SDL_INIT_VIDEO);

    WINDOW = SDL_CreateWindow
    (
        title,
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width, height,
        SDL_WINDOW_SHOWN
    );

    if (!WINDOW)
    {
        fputs("Could not create window.", stderr);
        exit(EXIT_FAILURE);
    }

    RENDERER = SDL_CreateRenderer(WINDOW, -1, 0);

    if (!RENDERER)
    {
        fputs("Could not create renderer.", stderr);
        exit(EXIT_FAILURE);
    }
}


void bz_present()
{
    SDL_Event e;
    while (1)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                return;
            }

            if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                is_mouse_button_down = 1;
            }

            if (e.type == SDL_MOUSEBUTTONUP)
            {
                is_mouse_button_down = 0;
            }

            if (e.type == SDL_MOUSEMOTION)
            {
                if (!is_mouse_button_down) continue;

                if (check_mouse_rect_collision(P1, 20))
                {
                    SDL_GetMouseState(&P1.x, &P1.y);
                }
                else if (check_mouse_rect_collision(P2, 20))
                {
                    SDL_GetMouseState(&P2.x, &P2.y);
                }
                else if (check_mouse_rect_collision(P3, 20))
                {
                    SDL_GetMouseState(&P3.x, &P3.y);
                }
            }
        }

        bezier();
    }
}

static void bezier()
{
    SDL_SetRenderDrawColor(RENDERER, 0xff, 0xff, 0xff, 0xff);
    SDL_RenderClear(RENDERER);

    SDL_Rect r_p1 = {
        .x = P1.x - 10,
        .y = P1.y - 10,
        .w = 20,
        .h = 20
    };

    SDL_SetRenderDrawColor(RENDERER, 0xff, 0x0, 0x0, 0xff);
    SDL_RenderDrawRect(RENDERER, &r_p1);

    SDL_Rect r_p3 = {
        .x = P3.x - 10,
        .y = P3.y - 10,
        .w = 20,
        .h = 20
    };

    SDL_SetRenderDrawColor(RENDERER, 0x0, 0x0, 0xff, 0xff);
    SDL_RenderDrawRect(RENDERER, &r_p3);

    SDL_Rect r_p2 = {
        .x = P2.x - 10,
        .y = P2.y - 10,
        .w = 20,
        .h = 20
    };

    SDL_SetRenderDrawColor(RENDERER, 0x0, 0x0, 0x0, 0xff);
    SDL_RenderDrawRect(RENDERER, &r_p2);

    const int N = 1e3;

    for (int i = 0; i < N; i++)
    {
        float t = (i + 1) / (float)(N);
        float alpha = 1 - t;

        int x1 = alpha * P1.x + t * P2.x;
        int y1 = alpha * P1.y + t * P2.y;

        int x2 = alpha * P2.x + t * P3.x;
        int y2 = alpha * P2.y + t * P3.y;

        int x = alpha * x1 + t * x2;
        int y = alpha * y1 + t * y2;

        SDL_SetRenderDrawColor(RENDERER, alpha * 0xff, 0, t * 0xff, 0xff);

        SDL_RenderDrawPoint(RENDERER, x, y);
    }

    SDL_RenderPresent(RENDERER);
}

void bz_terminate()
{
    SDL_DestroyRenderer(RENDERER);
    SDL_DestroyWindow(WINDOW);
    SDL_Quit();
}
