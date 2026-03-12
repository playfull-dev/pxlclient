#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <string>

// For opening browser (platform specific)
#ifdef _WIN32
    #include <windows.h>
    #define OPEN_URL(url) ShellExecuteA(NULL, "open", url, NULL, NULL, SW_SHOWNORMAL)
#elif __APPLE__
    #define OPEN_URL(url) system(("open " + std::string(url)).c_str())
#else
    #define OPEN_URL(url) system(("xdg-open " + std::string(url)).c_str())
#endif

int main()
{
    sf::RenderWindow window(sf::VideoMode(880, 720), "PXL Client", sf::Style::Close);
    window.setFramerateLimit(60);

    // ────────────────────────────────────────────────
    //  Fonts & Textures
    // ────────────────────────────────────────────────
    sf::Font font;
    if (!font.loadFromFile("fonts/arial.ttf")) {  // ← change to your font path
        std::cerr << "Failed to load font!\n";
        // You can continue without font, but text won't show
    }

    sf::Texture logoTexture;
    if (!logoTexture.loadFromFile("asset/PXLClient.png")) {
        std::cerr << "Failed to load PXLClient.png\n";
    }

    // ────────────────────────────────────────────────
    //  Sprites & Texts
    // ────────────────────────────────────────────────
    sf::Sprite logo;
    logo.setTexture(logoTexture);
    
    // Scale logo to almost window width (with some margin)
    float scale = (window.getSize().x - 80.f) / logoTexture.getSize().x;
    logo.setScale(scale, scale);
    logo.setPosition(40.f, 40.f);

    sf::Text title("Digital PXL - PXL perfect", font, 38);
    title.setFillColor(sf::Color(220, 220, 255));
    title.setStyle(sf::Text::Bold);
    sf::FloatRect titleBounds = title.getLocalBounds();
    title.setOrigin(titleBounds.width/2, titleBounds.height/2);
    title.setPosition(window.getSize().x/2.f, logo.getPosition().y + logo.getGlobalBounds().height + 60.f);

    sf::Text subtitle("PXL Client", font, 28);
    subtitle.setFillColor(sf::Color(160, 160, 220));
    subtitle.setOrigin(subtitle.getLocalBounds().width/2, subtitle.getLocalBounds().height/2);
    subtitle.setPosition(window.getSize().x/2.f, title.getPosition().y + 70.f);

    // ────────────────────────────────────────────────
    //  Button
    // ────────────────────────────────────────────────
    sf::RectangleShape button(sf::Vector2f(360, 90));
    button.setOrigin(button.getSize().x/2, button.getSize().y/2);
    button.setPosition(window.getSize().x/2.f, window.getSize().y - 160.f);
    button.setFillColor(sf::Color::Transparent);

    // We'll draw gradient manually using a simple two-color approach
    sf::VertexArray buttonGradient(sf::Quads, 4);
    buttonGradient[0].position = button.getPosition() - button.getOrigin();
    buttonGradient[1].position = buttonGradient[0].position + sf::Vector2f(button.getSize().x, 0);
    buttonGradient[2].position = buttonGradient[1].position + sf::Vector2f(0, button.getSize().y);
    buttonGradient[3].position = buttonGradient[0].position + sf::Vector2f(0, button.getSize().y);

    // Indigo → Purple gradient
    buttonGradient[0].color = sf::Color(99, 102, 241);
    buttonGradient[1].color = sf::Color(139, 92, 246);
    buttonGradient[2].color = sf::Color(139, 92, 246);
    buttonGradient[3].color = sf::Color(99, 102, 241);

    sf::Text buttonText("Download PXL", font, 42);
    buttonText.setFillColor(sf::Color::White);
    buttonText.setStyle(sf::Text::Bold);
    buttonText.setOrigin(buttonText.getLocalBounds().width/2, buttonText.getLocalBounds().height/2);
    buttonText.setPosition(button.getPosition());

    // Hover / press state
    bool isHovered = false;
    bool isPressed = false;

    // ────────────────────────────────────────────────
    //  Main loop
    // ────────────────────────────────────────────────
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                if (button.getGlobalBounds().contains(mousePos.x, mousePos.y))
                {
                    isPressed = true;

                    // Open browser to download link
                    const char* downloadUrl = "https://example.com/my-awesome-project.zip";
                    std::cout << "Opening download page: " << downloadUrl << "\n";
                    OPEN_URL(downloadUrl);
                }
            }

            if (event.type == sf::Event::MouseButtonReleased)
            {
                isPressed = false;
            }
        }

        // Hover detection
        sf::Vector2i mouse = sf::Mouse::getPosition(window);
        isHovered = button.getGlobalBounds().contains(mouse.x, mouse.y);

        // ─── Drawing ─────────────────────────────────────
        window.clear(sf::Color(9,9,11));   // #09090b

        window.draw(logo);
        window.draw(title);
        window.draw(subtitle);

        // Button background (gradient)
        float hoverOffset = isHovered ? -4.f : 0.f;
        float pressOffset = isPressed ? +6.f : 0.f;

        // Shadow / glow simulation (simple)
        sf::RectangleShape shadow = button;
        shadow.setFillColor(sf::Color(40,40,80,80));
        shadow.move(0, 14 + pressOffset);
        shadow.setSize(button.getSize() + sf::Vector2f(20,20));
        shadow.setOrigin(shadow.getSize()/2.f);
        shadow.setPosition(button.getPosition());

        window.draw(shadow);

        // Move gradient + text when pressed/hovered
        buttonGradient[0].position = button.getPosition() - button.getOrigin() + sf::Vector2f(0, hoverOffset + pressOffset);
        buttonGradient[1].position = buttonGradient[0].position + sf::Vector2f(button.getSize().x, 0);
        buttonGradient[2].position = buttonGradient[1].position + sf::Vector2f(0, button.getSize().y);
        buttonGradient[3].position = buttonGradient[0].position + sf::Vector2f(0, button.getSize().y);

        buttonText.setPosition(button.getPosition() + sf::Vector2f(0, hoverOffset + pressOffset));

        window.draw(buttonGradient);
        window.draw(buttonText);

        window.display();
    }

    return 0;
}
