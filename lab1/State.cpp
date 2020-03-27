#include "State.h"

bool State::Search(std::vector<State*>& v, State* what)
{
	if (std::find(v.begin(), v.end(), what) != v.end())
		return true;
	else return false;
}

//bool State::Repeats(std::vector<State*>& v)
//{
//	/*for (size_t i = 0; i < v.size(); i++)
//	{
//		for (size_t j = i + 1; j < v.size(); j++)
//		{
//			v.
//			if (*v[i] == *v[j])
//				return true;
//		}
//	}*/
//	return false;
//}

GameQuadCells& State::getGameQuadCellsNumber()
{
	return gameQuadCells;
}

void State::setGameQuadCellsNumber(GameQuadCells gqc)
{
	gameQuadCells.setC1(gqc.getC1());
	gameQuadCells.setC2(gqc.getC2());
	gameQuadCells.setC3(gqc.getC3());
	gameQuadCells.setC4(gqc.getC4());
}

State* State::getParent() const
{
	return parent;
}
State::State(char** ballsPositions, GameQuadCells gameQuadCells)
{
	if (ballsPositions != nullptr)
	{
		this->ballsPositions = new char* [5];
		for (size_t i = 0; i < 5; i++)
		{
			this->ballsPositions[i] = new char[5];
		}
		for (size_t i = 0; i < 5; i++)
		{
			for (size_t j = 0; j < 5; j++)
			{
				this->ballsPositions[i][j] = ballsPositions[i][j];
			}
		}
	}
	else this->ballsPositions = ballsPositions;	
	this->gameQuadCells = gameQuadCells;
}

State::State(const State& state)
{	
	if (ballsPositions == nullptr)
	{
		ballsPositions = new char* [5];
		for (size_t i = 0; i < 5; i++)
		{
			ballsPositions[i] = new char[5];
		}
	}
	for (size_t i = 0; i < 5; i++)
	{
		for (size_t j = 0; j < 5; j++)
		{
			this->ballsPositions[i][j] = state.ballsPositions[i][j];
		}
	}
	this->gameQuadCells = state.gameQuadCells;	
	this->parent = state.parent;	
}

State::State(State&& state) noexcept
{
	if (ballsPositions == nullptr)
	{
		ballsPositions = new char* [5];
		for (size_t i = 0; i < 5; i++)
		{
			ballsPositions[i] = new char[5];
		}
	}
	for (size_t i = 0; i < 5; i++)
	{
		for (size_t j = 0; j < 5; j++)
		{
			this->ballsPositions[i][j] = state.ballsPositions[i][j];
		}
	}
	for (size_t i = 0; i < 5; i++)
	{
		delete[] state.ballsPositions[i];
	}
	this->gameQuadCells = state.gameQuadCells;
	this->parent = new State(*state.parent);
	state.getGameQuadCellsNumber().setC1(0);
	state.getGameQuadCellsNumber().setC2(0);
	state.getGameQuadCellsNumber().setC3(0);
	state.getGameQuadCellsNumber().setC4(0);
	delete state.parent;
	state.parent = nullptr;
}

State& State::operator=(const State& state)
{
	if (state == *this)
		return *this;

	if (ballsPositions == nullptr)
	{
		ballsPositions = new char* [5];
		for (size_t i = 0; i < 5; i++)
		{
			ballsPositions[i] = new char[5];
		}
	}
	for (size_t i = 0; i < 5; i++)
	{
		for (size_t j = 0; j < 5; j++)
		{
			this->ballsPositions[i][j] = state.ballsPositions[i][j];
		}
	}
	this->gameQuadCells = state.gameQuadCells;
	this->parent = state.parent;
	return *this;
}

char** State::getBallsPositions() const
{
	return ballsPositions;
}
void State::Expand(std::vector<State*>& O, std::vector<State*>& C)
{
	int verticalPos = 0, horizontalPos = 0;
	char c1, c2, c3, c4;
		
	for (verticalPos = 0; verticalPos < 4; verticalPos++)
	{
		/*std::cout << "Parent" << std::endl << *this << std::endl;*/
		for (horizontalPos = 0; horizontalPos < 4; horizontalPos++)
		{
			c1 = ballsPositions[verticalPos % 5][horizontalPos % 5];
			c2 = ballsPositions[verticalPos % 5][horizontalPos % 5 + 1];
			c3 = ballsPositions[verticalPos % 5 + 1][horizontalPos % 5];
			c4 = ballsPositions[verticalPos % 5 + 1][horizontalPos % 5 + 1];

			int a = c1 + c2 + c3 + c4;
			if (a == 48 * 4 || a == 49 * 4)
				continue;

			State* state = new State(ballsPositions);
			//c2 = c1
			state->ballsPositions[verticalPos % 5][horizontalPos % 5 + 1] = c1;
			//c4 = c2
			state->ballsPositions[verticalPos % 5 + 1][horizontalPos % 5 + 1] = c2;
			//c1 = c3
			state->ballsPositions[verticalPos % 5][horizontalPos % 5] = c3;
			//c3 = c4			
			state->ballsPositions[verticalPos % 5 + 1][horizontalPos % 5] = c4;
			/*std::cout << "Child" << std::endl << *state<< std::endl;		*/			
			if ((Search(O, state) == false) && (Search(C, state) == false))
			{				
				state->parent = this;
				GameQuadCells gqc(verticalPos * 5 + horizontalPos, verticalPos * 5 + horizontalPos + 1, (verticalPos + 1) * 5 + horizontalPos, (verticalPos + 1) * 5 + horizontalPos + 1);
				state->setGameQuadCellsNumber(gqc);
				O.push_back(state);				
			}
			else delete state;
			
		}
	}	
}
void State::setParent(State* state)
{
	if (state != nullptr)
	parent = new State(*state);
	else
	parent = nullptr;
}
State::~State()
{
	if (ballsPositions != nullptr)
	{
		for (size_t i = 0; i < 5; i++)
		{
			delete[] ballsPositions[i];
		}
		delete[] ballsPositions;
	}
}

bool State::operator==(const State& rhs) const
{	
	if (ballsPositions == nullptr || rhs.ballsPositions == nullptr) return false;
	for (size_t i = 0; i < 5; i++)
	{
		for (size_t j = 0; j < 5; j++)
		{
			if (ballsPositions[i][j] != rhs.ballsPositions[i][j])
				return false;
		}
	}
	return true;
}

bool State::operator!=(const State& rhs) const
{
	return (!(*this == rhs));
}

std::ostream& operator<<(std::ostream& out, const State& state)
{
	out << std::endl;
	for (size_t i = 0; i < 5; i++)
	{
		for (size_t j = 0; j < 5; j++)
		{
			out << state.ballsPositions[i][j] << ' ';
		}
		out << std::endl;
	}
	return out;
}
