#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

const sf::Color PURPLE(128, 0, 128);
const sf::Color ORANGE(255, 165, 0);
const sf::Color DARK_GREEN(0, 100, 0);
const sf::Color YELLOW(255, 255, 0);

class Button
{

private:
    sf::RectangleShape box;
    sf::Text label;

    void adjustTextToFit();


public:
    Button(const std::string& text, sf::Font& font, const sf::Vector2f& position, const sf::Vector2f& size)
    {
        box.setPosition(position);
        box.setSize(size);
        box.setFillColor(ORANGE);
        box.setOutlineColor(sf::Color::Black);
        box.setOutlineThickness(3);

        label.setFont(font);
        label.setString(text);
        label.setFillColor(sf::Color::Black);

        adjustTextToFit();
    }

    void draw(sf::RenderWindow& window);

    bool isMouseOver(sf::RenderWindow& window) const;

};



void Button::adjustTextToFit()
{
    sf::FloatRect boxBounds = box.getGlobalBounds();
    float maxWidth = boxBounds.width - 10; 
    float maxHeight = boxBounds.height - 10; 

    unsigned int charSize = 12;
    label.setCharacterSize(charSize);
    sf::FloatRect textBounds = label.getLocalBounds();

    while ((textBounds.width > maxWidth || textBounds.height > maxHeight) && charSize > 1) {
        charSize--;
        label.setCharacterSize(charSize);
        textBounds = label.getLocalBounds();
    }

    if (textBounds.width > maxWidth)
    {
        std::string text = label.getString();
        while (textBounds.width > maxWidth && !text.empty())
        {
            text.pop_back();
            label.setString(text + "...");
            textBounds = label.getLocalBounds();
        }
    }

   
    label.setPosition(box.getPosition().x + 5, box.getPosition().y + (box.getSize().y - textBounds.height) / 2 - 5);
}


void Button::draw(sf::RenderWindow& window) {
    window.draw(box);
    window.draw(label);
}

bool Button::isMouseOver(sf::RenderWindow& window) const {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    return box.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos));
}






class TextBox
{

private:
    sf::RectangleShape box;
    sf::Text text;
    bool selected;
public:
    TextBox(sf::Font& font, const sf::Vector2f& position, const sf::Vector2f& size);

    void draw(sf::RenderWindow& window);
    void handleEvent(sf::Event event, sf::RenderWindow& window);
    std::string getText() const;

};

TextBox::TextBox(sf::Font& font, const sf::Vector2f& position, const sf::Vector2f& size) : selected(false) {
    box.setPosition(position);
    box.setSize(size);
    box.setFillColor(sf::Color::White);
    box.setOutlineColor(sf::Color::Black);
    box.setOutlineThickness(3); 

    text.setFont(font);
    text.setCharacterSize(20);
    text.setFillColor(sf::Color::Black);
    text.setPosition(position.x + 5, position.y + 5);
}

void TextBox::draw(sf::RenderWindow& window) 
{
    window.draw(box);
    window.draw(text);
}

void TextBox::handleEvent(sf::Event event, sf::RenderWindow& window)
{
    if (event.type == sf::Event::MouseButtonPressed)
    {
        if (box.getGlobalBounds().contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)))) {
            selected = true;
        }
        else {
            selected = false;
        }
    }

    if (selected && event.type == sf::Event::TextEntered) 
    {
        if (event.text.unicode == '\b')
        {
            if (!text.getString().isEmpty()) 
            {
                std::string str = text.getString();
                str.pop_back();
                text.setString(str);
            }
        }
        else if (event.text.unicode < 128 && text.getGlobalBounds().width < box.getSize().x - 10)
        {
            text.setString(text.getString() + static_cast<char>(event.text.unicode));
        }
    }
}

std::string TextBox::getText() const
{
    return text.getString();
}



struct TreeNode 
{
    int value;
    TreeNode* left;
    TreeNode* right;

    TreeNode(int v) : value(v), left(nullptr), right(nullptr) {}
};


class BST 
{


private:
    TreeNode* root;
    void insert(TreeNode*& node, int value);
    void remove(TreeNode*& node, int value);
    bool search(TreeNode* node, int value);
    TreeNode* findMin(TreeNode* node);
    void clear(TreeNode*& node);
    void inorder(TreeNode* node, vector<int>& result);
    void preorder(TreeNode* node, vector<int>& result);
    void postorder(TreeNode* node, vector<int>& result);
    void drawTree(TreeNode* node, sf::RenderWindow& window, sf::Font& font, int x, int y, int xOffset);

public:
    BST();
    void insert(int value);
    void remove(int value);
    bool search(int value);
    void clear();
    vector<int> inorder();
    vector<int> preorder();
    vector<int> postorder();
    void draw(sf::RenderWindow& window, sf::Font& font);

};

BST::BST() : root(nullptr) {}

void BST::insert(int value) 
{
    insert(root, value);
}

void BST::insert(TreeNode*& node, int value)
{
    if (!node) 
    {
        node = new TreeNode(value);
    }
    else if (value < node->value) 
    {
        insert(node->left, value);
    }
    else 
    {
        insert(node->right, value);
    }
}

void BST::remove(int value)
{
    remove(root, value);
}

void BST::remove(TreeNode*& node, int value)
{
    if (!node) return;

    if (value < node->value)
    {
        remove(node->left, value);
    }
    else if (value > node->value)
    {
        remove(node->right, value);
    }
    else {
        if (!node->left) 
        {
            TreeNode* temp = node;
            node = node->right;
            delete temp;
        }
        else if (!node->right) 
        {
            TreeNode* temp = node;
            node = node->left;
            delete temp;
        }
        else 
        {
            TreeNode* temp = findMin(node->right);
            node->value = temp->value;
            remove(node->right, temp->value);
        }
    }
}

TreeNode* BST::findMin(TreeNode* node) 
{
    while (node->left)
    {
        node = node->left;
    }
    return node;
}

bool BST::search(int value)
{
    return search(root, value);
}

bool BST::search(TreeNode* node, int value)
{
    if (!node) 
    {
        return false;
    }
    else if (node->value == value)
    {
        return true;
    }
    else if (value < node->value) 
    {
        return search(node->left, value);
    }
    else
    {
        return search(node->right, value);
    }
}

void BST::clear() 
{
    clear(root);
    root = nullptr;
}

void BST::clear(TreeNode*& node)
{
    if (node)
    {
        clear(node->left);
        clear(node->right);
        delete node;
    }
}

vector<int> BST::inorder()
{
    vector<int> result;
    inorder(root, result);
    return result;
}

void BST::inorder(TreeNode* node, vector<int>& result)
{
    if (node)
    {
        inorder(node->left, result);
        result.push_back(node->value);
        inorder(node->right, result);
    }
}

vector<int> BST::preorder() 
{
    vector<int> result;
    preorder(root, result);
    return result;
}

void BST::preorder(TreeNode* node, vector<int>& result) 
{
    if (node) 
    {
        result.push_back(node->value);
        preorder(node->left, result);
        preorder(node->right, result);
    }
}

vector<int> BST::postorder()
{
    vector<int> result;
    postorder(root, result);
    return result;
}

void BST::postorder(TreeNode* node, vector<int>& result)
{
    if (node) 
    {
        postorder(node->left, result);
        postorder(node->right, result);
        result.push_back(node->value);
    }
}

void BST::draw(sf::RenderWindow& window, sf::Font& font) 
{
    if (root)
    {
        drawTree(root, window, font, window.getSize().x / 2, 150, window.getSize().x / 4);
    }
}

void BST::drawTree(TreeNode* node, sf::RenderWindow& window, sf::Font& font, int x, int y, int xOffset) {
    if (node)
    {
        sf::CircleShape circle(30);
        circle.setFillColor(sf::Color::Yellow); 
        circle.setPosition(static_cast<float>(x - circle.getRadius()), static_cast<float>(y - circle.getRadius()));
        window.draw(circle);

        sf::Text text;
        text.setFont(font);
        text.setString(to_string(node->value));
        text.setCharacterSize(20);
        text.setFillColor(sf::Color::Black);
        sf::FloatRect textRect = text.getLocalBounds();
        text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
        text.setPosition(static_cast<float>(x), static_cast<float>(y));
        window.draw(text);

        if (node->left)
        {
            sf::Vertex line[] =
            {
                sf::Vertex(sf::Vector2f(static_cast<float>(x), static_cast<float>(y + circle.getRadius())), PURPLE),
                sf::Vertex(sf::Vector2f(static_cast<float>(x - xOffset), static_cast<float>(y + 100 - circle.getRadius())), PURPLE)
            };

            line[0].color = line[1].color = PURPLE; 
            line[0].position.y += 5; 
            line[1].position.y -= 5; 
            window.draw(line, 2, sf::Lines);
            drawTree(node->left, window, font, x - xOffset, y + 100, xOffset / 2);
        }

        if (node->right) 
        {
            sf::Vertex line[] =
            {
                sf::Vertex(sf::Vector2f(static_cast<float>(x), static_cast<float>(y + circle.getRadius())), PURPLE),
                sf::Vertex(sf::Vector2f(static_cast<float>(x + xOffset), static_cast<float>(y + 100 - circle.getRadius())), PURPLE)
            };

            line[0].color = line[1].color = PURPLE; 
            line[0].position.y += 5; 
            line[1].position.y -= 5;
            window.draw(line, 2, sf::Lines);
            drawTree(node->right, window, font, x + xOffset, y + 100, xOffset / 2);
        }
    }
}


class BSTVisualizer
{
private:
    BST& tree;
    sf::RenderWindow window;
    sf::Font font;
    sf::Font titleFont;

    TextBox insertBox;
    TextBox deleteBox;
    TextBox searchBox;
    Button insertButton;
    Button deleteButton;
    Button searchButton;
    Button inorderButton;
    Button preorderButton;
    Button postorderButton;
    Button clearButton;

    string traversalResult;

    void draw();
    void handleEvent(sf::Event event);

public:
    BSTVisualizer(BST& tree);

    void run();

};

BSTVisualizer::BSTVisualizer(BST& tree)
    : tree(tree), window(sf::VideoMode::getDesktopMode(), "BST Visualizer", sf::Style::Fullscreen),
    insertBox(font, { 150, 60 }, { 80, 30 }),   
    deleteBox(font, { 150, 100 }, { 80, 30 }),   
    searchBox(font, { 150, 140 }, { 80, 30 }),   
    insertButton("Insert", font, { 50, 60 }, { 100, 30 }), 
    deleteButton("Delete", font, { 50, 100 }, { 100, 30 }),
    searchButton("Search", font, { 50, 140 }, { 100, 30 }), 
    inorderButton("Inorder", font, { 260, 60 }, { 100, 30 }), 
    preorderButton("Preorder", font, { 370, 60 }, { 100, 30 }), 
    postorderButton("Postorder", font, { 480, 60 }, { 100, 30 }), 
    clearButton("Clear", font, { 590, 60 }, { 100, 30 }), 
    traversalResult("")
{

    if (!font.loadFromFile("C:\\Users\\Ali\\source\\repos\\BST GV\\font\\arial.ttf")) 
    {
        cerr << "Could not load font\n";
    }
    if (!titleFont.loadFromFile("C:\\Users\\Ali\\source\\repos\\BST GV\\font\\bst.ttf")) 
    { 
        cerr << "Could not load title font\n";
    }
}

void BSTVisualizer::run()
{
    while (window.isOpen()) 
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) 
            {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
            {
                window.close();
            }
            handleEvent(event);
        }
        window.clear(sf::Color::White); 
        draw();
        window.display();
    }
}

void BSTVisualizer::draw()
{
    insertBox.draw(window);
    deleteBox.draw(window);
    searchBox.draw(window);
    insertButton.draw(window);
    deleteButton.draw(window);
    searchButton.draw(window);
    inorderButton.draw(window);
    preorderButton.draw(window);
    postorderButton.draw(window);
    clearButton.draw(window);
    tree.draw(window, font);

    sf::RectangleShape titleStrip(sf::Vector2f(window.getSize().x, 50));
    titleStrip.setFillColor(DARK_GREEN); 
    titleStrip.setPosition(0, 0);
    window.draw(titleStrip);

    sf::Text title("BST Visualizer", titleFont, 30); 
    title.setFillColor(YELLOW); 
    title.setPosition(window.getSize().x / 2 - title.getGlobalBounds().width / 2, 10);
    window.draw(title);

    sf::Text resultText(traversalResult, font, 20);
    resultText.setFillColor(sf::Color::Black);
    resultText.setPosition(10, window.getSize().y - 40); 
    window.draw(resultText);
}

void BSTVisualizer::handleEvent(sf::Event event)
{
    insertBox.handleEvent(event, window);
    deleteBox.handleEvent(event, window);
    searchBox.handleEvent(event, window);

    if (event.type == sf::Event::MouseButtonPressed)
    {
        if (insertButton.isMouseOver(window)) 
        {
            int value = stoi(insertBox.getText());
            tree.insert(value);
        }
        if (deleteButton.isMouseOver(window)) 
        {
            int value = stoi(deleteBox.getText());
            tree.remove(value);
        }
        if (searchButton.isMouseOver(window)) 
        {
            int value = stoi(searchBox.getText());
            bool found = tree.search(value);
            traversalResult = found ? "Value found in tree" : "Value not found in tree";
        }
        if (inorderButton.isMouseOver(window))
        {
            vector<int> result = tree.inorder();
            traversalResult = "Inorder: ";
            for (int val : result) {
                traversalResult += to_string(val) + " ";
            }
        }
        if (preorderButton.isMouseOver(window))
        {
            vector<int> result = tree.preorder();
            traversalResult = "Preorder: ";
            for (int val : result) {
                traversalResult += to_string(val) + " ";
            }
        }
        if (postorderButton.isMouseOver(window))
        {
            vector<int> result = tree.postorder();
            traversalResult = "Postorder: ";
            for (int val : result) {
                traversalResult += to_string(val) + " ";
            }
        }
        if (clearButton.isMouseOver(window))
        {
            tree.clear();
            traversalResult = "Tree cleared.";
        }
    }
}


int main()
{
    BST tree;
    BSTVisualizer visualizer(tree);
    visualizer.run();
    return 0;
}
