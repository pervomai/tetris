#include "Tetris.h"

/*
 0 1 2 3
 4 5 6 7 */

const int Tetris::figure[7][4] =
{
    0, 1, 2, 3, // I
    0, 1, 5, 6, // Z
    0, 4, 5, 6, // J
    2, 6, 5, 4, // L
    1, 2, 5, 6, // O
    2, 1, 5, 4, // S
    1, 4, 5, 6  // T
};

bool Tetris::selectRect(SDL_Rect& rect, const int x, const int y)
{
    if (x >= rect.x && x <= rect.x + rect.w && y >= rect.y && y <= rect.y + rect.h)
        return true;
    return false;
}

bool Tetris::menu()
{
    SDL_Event e;
    backgroundMenu = loadTexture("src/menu_"+to_string(language)+".png");
    play = loadTexture("src/play_" + to_string(language) + ".png");
    play_light = loadTexture("src/play_light_" + to_string(language) + ".png");
    record = loadTexture("src/record_" + to_string(language) + ".png");
    record_light = loadTexture("src/record_light_" + to_string(language) + ".png");
    rule = loadTexture("src/rules_" + to_string(language) + ".png");
    rule_light = loadTexture("src/rules_light_" + to_string(language) + ".png");
    english = loadTexture("src/english.png");
    russian = loadTexture("src/russian.png");

    SDL_Rect playRect = { 30, 250, 191, 54 }; //x,y,w,h
    SDL_Rect recordRect = { 30, 350, 191, 54 };
    SDL_Rect ruleRect = { 30, 450, 191, 54 };
    SDL_Rect langRect = { 400, 50, 76, 54 };
    SDL_RenderCopy(renderer, backgroundMenu, NULL, NULL);
    SDL_RenderCopy(renderer, play, NULL, &playRect);
    SDL_RenderCopy(renderer, record, NULL, &recordRect);
    SDL_RenderCopy(renderer, rule, NULL, &ruleRect);
    if (language == 1)
    {
        SDL_RenderCopy(renderer, english, NULL, &langRect);
    }
    else
    {
        SDL_RenderCopy(renderer, russian, NULL, &langRect);
    }

    int x = 0;
    int y = 0;
    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_QUIT)
        {
            return false;
        }
        else if (e.type == SDL_MOUSEMOTION)
        {
            x = e.motion.x;
            y = e.motion.y;
            if (selectRect(playRect, x, y))
            {
                SDL_RenderCopy(renderer, play_light, NULL, &playRect);
            }
            else if (selectRect(recordRect, x, y))
            {
                SDL_RenderCopy(renderer, record_light, NULL, &recordRect);
            }
            else if (selectRect(ruleRect, x, y))
            {
                SDL_RenderCopy(renderer, rule_light, NULL, &ruleRect);
            }
        }
        else if (e.type == SDL_MOUSEBUTTONDOWN)
        {
            if (e.button.button == SDL_BUTTON_LEFT)
            {
                x = e.button.x;
                y = e.button.y;
                if (selectRect(playRect, x, y))
                {
                    running = true;
                }
                else if (selectRect(recordRect, x, y))
                {
                    while(showRecord());
                }
                else if (selectRect(ruleRect, x, y))
                {
                    while (showRule());
                }
                else if (selectRect(langRect, x, y))
                {
                    if (language == 1) 
                    {
                        language = 2;
                    }
                    else 
                    {
                        language = 1;
                    }

                }
            }
        }
    }

    SDL_RenderPresent(renderer);
    return true;
}

bool Tetris::showRecord()
{
    SDL_Event e;
    tableImage = loadTexture("src/recordTable_" + to_string(language) + ".png");
    SDL_RenderCopy(renderer, tableImage, NULL, NULL);
    exit = loadTexture("src/exit.png");
    exitRect = { 400, 40, 54, 54 }; //x,y,w,h
    SDL_RenderCopy(renderer, exit, NULL, &exitRect);
    int x = 0;
    int y = 0;
    int exy = 275;

    for (int i = 0; i < 5; i++) 
    {
        if (recordTable[i] != 0)
        {
            string text = to_string(recordTable[i]);
            makeText(text, 230, exy, { 33, 56, 174 });
            exy += 60;
            SDL_RenderCopy(renderer, Text, &srcTextRect, &desTextRect);
        }
    }

    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_QUIT)
        {
            return false;
        }
        else if (e.type == SDL_MOUSEBUTTONDOWN)
        {
            if (e.button.button == SDL_BUTTON_LEFT)
            {
                x = e.button.x;
                y = e.button.y;
                if (selectRect(exitRect, x, y))
                {
                    return false;
                }
            }
        }
    }
    SDL_RenderPresent(renderer);
    clean();
    return true;
}

bool Tetris::showRule()
{
    SDL_Event e;
    int x = 0;
    int y = 0;
    tableImage = loadTexture("src/rulesTable.png");
    exit = loadTexture("src/exit.png");
    exitRect = { 400, 40, 54, 54 }; //x,y,w,h
    SDL_RenderCopy(renderer, tableImage, NULL, NULL);
    SDL_RenderCopy(renderer, exit, NULL, &exitRect);

    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_QUIT)
        {
            return false;
        }
        else if (e.type == SDL_MOUSEBUTTONDOWN)
        {
            if (e.button.button == SDL_BUTTON_LEFT)
            {
                x = e.button.x;
                y = e.button.y;
                if (selectRect(exitRect, x, y))
                {
                    return false;
                }
            }
        }
    }
    SDL_RenderPresent(renderer);
    clean();
    return true;
}

bool Tetris::init()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
    {
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
        window = SDL_CreateWindow(TITLE.c_str(), SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (window != NULL)
        {
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
            if (renderer != NULL)
            {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
                futureTetromino();
                nextTetromino();
                futureTetromino();
            }
            else
                return false;
        }
        else
            return false;
    }
    else
        return false;

    myFile.open("records.txt", ios::in);
    if (myFile.is_open())
    {
        string line;
        int i = 0;
        while (getline(myFile, line))
        {
            if (i < 5)
            {
                recordTable[i] = stoi(line);
            }
            i++;
        }
        myFile.close();
    }

    return true;
}

SDL_Texture* Tetris::loadTexture(string path)
{
    SDL_Texture* newTexture = nullptr;
    SDL_Surface* loadSurface = IMG_Load(path.c_str());
    newTexture = SDL_CreateTextureFromSurface(renderer, loadSurface);
    SDL_FreeSurface(loadSurface);
    return newTexture;
}

void Tetris::handleEvents()
{
    SDL_Event e;
    int x = 0;
    int y = 0;

    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_QUIT)
        {
            running = false;
            reset();
            break;
        }
        else if (e.type == SDL_MOUSEBUTTONDOWN)
        {
            if (e.button.button == SDL_BUTTON_LEFT)
            {
                x = e.button.x;
                y = e.button.y;
                if (selectRect(exitRect, x, y))
                {
                    running = false;
                    reset();
                    break;
                }
            }
        }
        else if (e.type == SDL_KEYDOWN)
        {
            switch (e.key.keysym.sym)
            {
            case SDLK_RIGHT:
            {
                move = 1;
                break;
            }
            case SDLK_LEFT:
            {
                move = -1;
                break;
            }
            case SDLK_UP:
            {
                rotate = true;
                break;
            }
            case SDLK_DOWN:
            {
                delay = 0;
            }
            }
        }
    }
}

void Tetris::setCurrentTime(Uint32 t)
{
    currentTime = t;
}

void Tetris::updateField(SDL_Rect& rect, int x, int y)
{
    rect.x += x;
    rect.y += y;
}

bool Tetris::checkCollision()
{
    for (int i = 0; i < 4; i++)
    {
        if (item[i].x < 0 || item[i].x >= col || item[i].y >= line)
            return false;
        else if (field[item[i].y][item[i].x])
            return false;
    }
    return true;
}

void Tetris::makeTetromino(SDL_Rect& rect, int x, int y, int w, int h)
{
    rect = { x, y, w, h };
}

void Tetris::futureTetromino() 
{
    futcolor = 1 + rand() % 7;
    int a = rand() % 7;
    for (int i = 0; i < 4; i++)
    {
        futitem[i].x = figure[a][i] % 4;
        futitem[i].y = figure[a][i] / 4;
    }
}

void Tetris::nextTetromino()
{
    color = futcolor;
    for (int i = 0; i < 4; i++)
    {
        item[i].x = futitem[i].x;
        item[i].y = futitem[i].y;
    }
}

void Tetris::gameplay()
{
    //возможность отката
    for (int i = 0; i < 4; i++)
        backup[i] = item[i];
    //вправо/влево
    for (int i = 0; i < 4; i++)
    {
        item[i].x += move;
    }
    if (!checkCollision())
    {
        for (int i = 0; i < 4; i++)
            item[i] = backup[i];
    }
    //поворот
    if (rotate)
    {
        point p = item[2];
        for (int i = 0; i < 4; i++)
        {
            int x = item[i].y - p.y;
            int y = item[i].x - p.x;
            item[i].x = p.x - x;
            item[i].y = p.y + y;
        }
        if (!checkCollision())
        {
            for (int i = 0; i < 4; i++)
                item[i] = backup[i];
        }
    }
    //падение
    if (currentTime - lastTime > delay)
    {
        for (int i = 0; i < 4; i++)
        {
            item[i].y += 1;
        }
        lastTime = currentTime;
        if (!checkCollision())
        {
            for (int i = 0; i < 4; i++)
                field[backup[i].y][backup[i].x] = color;
            nextTetromino();
            futureTetromino();
        }
    }
    if (currentTime / 1000 > 60)
        delay = 250;
    else if (currentTime / 1000 > 30)
        delay = 500;
    else
        delay = 900;

    move = 0;
    rotate = false;
}

void Tetris::makeText(string text, int x, int y, SDL_Color color)
{
	if (TTF_Init() == 0)
	{
		TTF_Font* font = NULL;
		font = TTF_OpenFont("src/mermaid.ttf", 30);

		SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
		SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
		Text = texture;
		SDL_FreeSurface(surface);
		TTF_SizeText(font, text.c_str(), &srcTextRect.w, &srcTextRect.h);

		srcTextRect.x = 0;
		srcTextRect.y = 0;

		desTextRect.x = x;
		desTextRect.y = y;

		desTextRect.w = srcTextRect.w;
		desTextRect.h = srcTextRect.h;
	}
}

void Tetris::checkLine()
{
    int k = line - 1;
    for (int i = k; i > 0; i--)
    {
        int count = 0;
        for (int j = 0; j < col; j++)
        {
            if (field[i][j])
                count++;
            field[k][j] = field[i][j];
        }
        if (count < col)
            k--;
        if (count == col)
        {
            score += 100;
            string text = to_string(score);
			makeText(text, 72, 100, { 33, 56, 174 });
        }
    }
}

void Tetris::checkGameOver()
{
    int filled_lines = 0;
    for (int i = 0; i < line; i++)
    {
        for (int j = 0; j < col; j++)
        {
            if (field[i][j] != 0)
            {
                filled_lines++;
                break;
            }
        }
    }

    if (filled_lines == line)
    {
        int temp = 0;
        int temp2 = 0;

        for (int i = 0; i < 5; i++)//ADD or CHANGE SCORE TABLE
        {
            if (score > recordTable[i]) 
            {
                temp = recordTable[i];
                recordTable[i] = score;
                if (i == 4) 
                {
                    break;
                }
                for (int j = i+1; j < 5; j++) //CHANGE NEXT NUMBERS IN TABLE
                {
                    temp2 = recordTable[j];
                    recordTable[j] = temp;
                    temp = temp2;
                }
                break;
            }
        }

        myFile.open("records.txt", ios::out);//FILE
        if (myFile.is_open()) 
        {
            string str1;
            for (int i = 0; i < 5; i++)
            {
                str1.append(to_string(recordTable[i]) + "\n");
            }
            myFile << str1;
            myFile.close();
        }

        running = false;
        reset();
		clean();
    }
}

void Tetris::updateRenderer()
{
    background = loadTexture("src/background_" + to_string(language) + ".png");
    block = loadTexture("src/blocks.png");
    exit = loadTexture("src/exit.png");
    exitRect = { 20, 10, 54, 54 }; //x,y,w,h
    SDL_RenderCopy(renderer, background, NULL, NULL);
    SDL_RenderCopy(renderer, exit, NULL, &exitRect);
    if (score == 0)
    {
        makeText("0", 88, 100, { 33, 56, 174 });
        SDL_RenderCopy(renderer, Text, &srcTextRect, &desTextRect);
    }
    SDL_RenderCopy(renderer, Text, &srcTextRect, &desTextRect);
    for (int i = 0; i < line; i++)
    {
        for (int j = 0; j < col; j++)
        {
            if (field[i][j])
            {
                makeTetromino(sRect, field[i][j] * 30);
                makeTetromino(dRect, j * blockW, i * blockH);

                updateField(dRect, blockW, SCREEN_HEIGHT - (line + 1) * blockH);
                SDL_RenderCopy(renderer, block, &sRect, &dRect);
            }
        }
    }
    for (int i = 0; i < 4; i++)
    {
        makeTetromino(sRect, color * 30);
        makeTetromino(dRect, item[i].x * blockW, item[i].y * blockH);

        makeTetromino(sFutRect, futcolor * 30);
        makeTetromino(dFutRect, futitem[i].x * blockW + 370, futitem[i].y * blockH + 50);

        updateField(dRect, blockW, SCREEN_HEIGHT - (line + 1) * blockH);
        SDL_RenderCopy(renderer, block, &sFutRect, &dFutRect);
        SDL_RenderCopy(renderer, block, &sRect, &dRect);
    };
    SDL_RenderPresent(renderer);
    clean();
}

void Tetris::reset()
{
    for (int i = 0; i < line; i++)
    {
        for (int j = 0; j < col; j++)
        {
            field[i][j] = 0;
        }
    }
    
    SDL_RenderPresent(renderer);
    clean();

    futureTetromino();
    nextTetromino();
    futureTetromino();
}

void Tetris::clean()
{
    SDL_DestroyTexture(background);
    SDL_DestroyTexture(block);
    SDL_DestroyTexture(backgroundMenu);
    SDL_DestroyTexture(play);
    SDL_DestroyTexture(record);
    SDL_DestroyTexture(rule);
    SDL_DestroyTexture(play_light);
    SDL_DestroyTexture(record_light);
    SDL_DestroyTexture(rule_light);
    SDL_DestroyTexture(english);
    SDL_DestroyTexture(russian);
    SDL_DestroyTexture(exit);
}

void Tetris::quitSDL()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}
