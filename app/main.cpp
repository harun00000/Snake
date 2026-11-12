#include "game.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include "config.hpp"
#include "snake.hpp"
#include <iostream>
#include <logger.hpp>
#include <SFML/Audio.hpp>

int loadHighScore(const std::string& filename);
void saveHighScore(const std::string& filename, int score);

int main(){
    // borders 
    int windowWidth = config::maxWidth * config::cellSize;
    int windowHeight = config::maxHeight * config::cellSize;

    // SFML window
    sf::RenderWindow window(
        sf::VideoMode(windowWidth, windowHeight),
        "Snake"
    );

    // game create
    Game game(config::maxWidth, config::maxHeight);

    enum class GameStatus{ 
        START, 
        PLAYING, 
        PAUSED, 
        GAMEOVER
    };
    GameStatus state = GameStatus::START;

    // high score
    const std::string highScoreFile = "highscore.txt";
    int highScore = loadHighScore(highScoreFile);
    int lastScore = 0;

    // logger
    Logger logger("game.log");
    logger.info("Game started");

    // timer 
    sf::Clock clock;
    float timer = 0.0f;

    // events
    sf::Event event;
    
    // head, body food create
    sf::Texture headTex, bodyTex, foodTex;
    headTex.loadFromFile("asserts/snake_head.png");
    bodyTex.loadFromFile("asserts/snake_body.png");
    foodTex.loadFromFile("asserts/snake_food.png");
    sf::Sprite headSprite(headTex), bodySprite(bodyTex), foodSprite(foodTex);
    
    // sounds
    sf::SoundBuffer eatBuffer;
    sf::SoundBuffer dieBuffer;
    eatBuffer.loadFromFile("asserts/eat.wav");
    dieBuffer.loadFromFile("asserts/die.wav");
    sf::Sound eatSound(eatBuffer), dieSound(dieBuffer);
    
    // font
    sf::Font font;
    if (!font.loadFromFile("asserts/arial_bolditalicmt.ttf"))
    {
        std::cout << "Font load error\n";
    }

    // score text
    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::White);

    // game over text
    sf::Text gameOverText;
    gameOverText.setFont(font);
    gameOverText.setCharacterSize(40);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setString("GAME OVER\nPress R");
    gameOverText.setPosition(
        windowWidth / 2 - 100,
        windowHeight / 2 - 50
    );

    // for pause
    bool isPaused = false;
    sf::Text pauseText;
    pauseText.setFont(font);
    pauseText.setCharacterSize(40);
    pauseText.setFillColor(sf::Color::White);
    pauseText.setString("PAUSED");
    pauseText.setPosition(windowWidth/2 - 80, windowHeight/2 - 20);
    
    // start text
    sf::Text startText;
    startText.setFont(font);
    startText.setCharacterSize(26);
    startText.setFillColor(sf::Color::Yellow);
    startText.setString("SNAKE\n\nPress SPACE to start");
    startText.setPosition(windowWidth / 2 - 130, windowHeight / 2 - 80);

    // high score
    sf::Text highScoreText;
    highScoreText.setFont(font);
    highScoreText.setCharacterSize(24);
    highScoreText.setFillColor(sf::Color(255, 215, 0));

    while (window.isOpen()){
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed)
            {
                // start
                if (state == GameStatus::START){
                    if (event.key.code == sf::Keyboard::Space) {
                        state = GameStatus::PLAYING;
                        logger.info("New game started");
                    }
                }

                // game move
                else if (state == GameStatus::PLAYING)
                {
                    if (event.key.code == sf::Keyboard::W)
                        game.getSnake().setNextMove(0, -1);
                    

                    if (event.key.code == sf::Keyboard::S)
                        game.getSnake().setNextMove(0, 1);
                    

                    if (event.key.code == sf::Keyboard::A)
                        game.getSnake().setNextMove(-1, 0);
                        

                    if (event.key.code == sf::Keyboard::D)
                        game.getSnake().setNextMove(1, 0);
                }

                // pause
                else if (state == GameStatus::PAUSED)
                {
                    if (event.key.code == sf::Keyboard::Space) 
                    {
                    state = GameStatus::PLAYING;
                    logger.info("Game resumed");
                    }
                }

                // gameover 
                else if (state == GameStatus::GAMEOVER)
                {
                    if (event.key.code == sf::Keyboard::R) 
                    {
                        game = Game(config::maxWidth, config::maxHeight);
                        state = GameStatus::PLAYING;
                        lastScore = 0;
                        logger.info("Game restarted");
                    }
                }
            }   
        }
        
        // add how many time has gone from the previous frame        
        float time = clock.restart().asSeconds();
        
        if (state == GameStatus::PLAYING)
        {
            timer += time;

            float speed = config::tickDelay - game.getScore() * 0.01f;
            if (speed < 0.05f) speed = 0.05f;

            while (timer > speed) {
                game.Update();
                timer -= speed;
            }

            // eat food log
            if (game.getScore() != lastScore) {
                lastScore = game.getScore();
                eatSound.play();
                logger.info("Food eaten, score: " + std::to_string(lastScore));
            }

            // gameover
            if (game.gameOver()) {
                state = GameStatus::GAMEOVER;
                dieSound.play();

                int score = game.getScore();
                logger.error("HA-HA-HA snake died, final score: " + std::to_string(score));
                
                if (score > highScore) 
                {
                    highScore = score;
                    saveHighScore(highScoreFile, highScore);
                    logger.info("New high score: " + std::to_string(highScore));
                }
            }
        }

        window.clear();

        if (state == GameStatus::START) window.draw(startText);
        else 
        {
            // draw snake
            const auto& body = game.getSnake().getBody();
            for (size_t idx = 0; idx < body.size(); idx++)
            {
                if (idx == 0)   // head
                {
                    headSprite.setPosition(
                        body[idx].x * config::cellSize,
                        body[idx].y * config::cellSize);
                    window.draw(headSprite);
                } 
                else 
                {               // body
                    bodySprite.setPosition(
                        body[idx].x * config::cellSize,
                        body[idx].y * config::cellSize);
                    window.draw(bodySprite);
                }
            }

            // food draw
            Point food = game.getFood().getCords();
            foodSprite.setPosition(
                food.x * config::cellSize,
                food.y * config::cellSize);
            window.draw(foodSprite);

            // counter
            scoreText.setString("Score: " + std::to_string(game.getScore()));
            scoreText.setPosition(5, 5);
            window.draw(scoreText);

            // high score 
            highScoreText.setString("Best: " + std::to_string(highScore));
            highScoreText.setPosition(windowWidth - 120, 5);
            window.draw(highScoreText);

            // game over
            if (state == GameStatus::GAMEOVER)
                window.draw(gameOverText);

            // pause
            if (state == GameStatus::PAUSED)
                window.draw(pauseText);
        }

        window.display();
    }

    return 0;
}

int loadHighScore(const std::string& filename){
    std::ifstream file(filename);
    int score = 0;

    if (file.is_open()) file >> score;
    return score;
}

void saveHighScore(const std::string& filename, int score){
    std::ofstream file(filename);
    if (file.is_open()) file << score;
}
