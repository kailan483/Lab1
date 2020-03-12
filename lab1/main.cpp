#include "GameField.h"
#include <fstream>
char** ReadFile(std::string filename)
{
	char** arr = new char*[5];
	for (size_t i = 0; i < 5; i++)
	{
		arr[i] = new char[5];
	}
	std::ifstream is;
	is.open(filename);
	char ch;
	int c = 0;
	if (is.is_open())
	{
		while ((ch = is.get()) != EOF)
		{
			if (ch != '\n')
			{
				arr[c / 5][c % 5] = ch;
				c++;
			}
			
		}
		return arr;
	}
	else return nullptr;
}

void Expand(State* parentState, std::vector<State*>& O, std::vector<State*>& C)
{
	parentState->Expand(O, C);
}

bool Search(std::vector<State*> q, State* what)
{	
	for (auto& i : q)
	{
		if (*i == *what)
			return true;
	}
	return false;
}
State* Solution(State* startState,State* targetState)
{
	using namespace std;
	std::vector<State*> O, C;	
	O.reserve(10000);
	C.reserve(10000);
	O.push_back(startState);
	
	bool searched = false;	
	clock_t start = clock();
	while (O.size() > 0)
	{		
		auto currentState = O.front();	
		if (*currentState == *targetState)
		{
			clock_t end = clock();
			double seconds = (double)(end - start) / CLOCKS_PER_SEC;
			cout << "The time of search is " << seconds << " seconds" << endl;
			cout << "Iterations count: " << (C.size()) << endl;
			return currentState;
		}								
		O.erase(O.begin());
		C.push_back(currentState);													
		currentState->Expand(O, C);
	}	
	return nullptr;
}
void print(char** arr, int w, int h)
{
	using namespace std;
	for (size_t i = 0; i < h; i++)
	{
		for (size_t j = 0; j < w; j++)
		{
			cout << arr[i][j] << ' ';
		}
		cout << endl;
	}
}
void cleararray(char** arr, int h)
{
	for (size_t i = 0; i < h; i++)
	{
		delete[] arr[i];
	}
	arr = nullptr;
}
int main()
{
	using namespace std;
	sf::RenderWindow window(sf::VideoMode(1024, 768), "SFML works!");
	window.setKeyRepeatEnabled(false);		
	char** ballsPositions = ReadFile("gameField.txt");	
	char** targetBallsPositions = ReadFile("target.txt");	
	State* startState = new State(ballsPositions,GameQuadCells(0,1,5,6));		
	State* gameBeginState = new State(*startState);	
	
	cleararray(ballsPositions, 5);

	GameQuad* gq = new GameQuad(5, 5);
	State* targetState = new State(targetBallsPositions);
	cleararray(targetBallsPositions, 5);

	GameField* gf = new GameField(120,80,gq,gameBeginState);		
	GameField* target = new GameField(560,80,nullptr,targetState);
	bool pause = false;
	int i;
	std::vector<State*> states;
	
	bool showSolution = false;	

	float time = 0;
	sf::Clock timer;
	timer.restart();
	bool moveQuad = false, changeBallsPositions = false;
	while (window.isOpen())
	{
		time += timer.getElapsedTime().asMilliseconds();
		timer.restart();		
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			else if (event.type == sf::Event::EventType::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Down)
					gf->moveGameQuad(Direction::Down);
				if (event.key.code == sf::Keyboard::Left)
					gf->moveGameQuad(Direction::Left);
				if (event.key.code == sf::Keyboard::Right)
					gf->moveGameQuad(Direction::Right);
				if (event.key.code == sf::Keyboard::Up)
					gf->moveGameQuad(Direction::Up);
				if (event.key.code == sf::Keyboard::Space)
				{					
					gf->turnBalls();					
				}
				if (event.key.code == sf::Keyboard::Enter)
				{
					if (showSolution == true)
					{
						gf->setBallsPositions(startState->getBallsPositions());
						gf->setGameQuadCells(startState->getGameQuadCellsNumber());
						showSolution = false;
					}
					else
					{
						
						states.clear();
						time = 0;
						
						State* s = Solution(startState, targetState);
						
						if (s == nullptr)
						{
							cout << "Solution not found!";
							return 1;
						}

						State* tmp = s;
						while (tmp != nullptr)
						{
							states.push_back(tmp);
							tmp = tmp->getParent();
						}
						if (*gameBeginState != *startState)
							i = states.size() - 1;
						else
							i = states.size() - 2;
						delete tmp;
						showSolution = true;
					}
					
				}
			}									
		}
		window.clear(sf::Color(247, 25, 69, 0));
		if (showSolution == true)
		{						
			if (i > -1)
			{
				if (time > 500 && !moveQuad)
				{
					gf->setGameQuadCells(states[i]->getGameQuadCellsNumber());
					gf->getState()->setGameQuadCellsNumber(states[i]->getGameQuadCellsNumber());
					moveQuad = true;
					changeBallsPositions = false;
				}
				if (time > 1000 && !changeBallsPositions)
				{
					gf->setBallsPositions(states[i]->getBallsPositions());
					time = 0;
					changeBallsPositions = true;
					moveQuad = false;
					i--;
				}
			}
			else i = states.size() - 1;
		}
		gf->Draw(&window);
		target->Draw(&window);
		window.display();
	}

	return 0;
}