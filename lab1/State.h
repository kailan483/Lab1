//#pragma once
#ifndef STATE_H
#define STATE_H
#include <vector>
#include <list>
#include <iostream>
#include <algorithm>
#include <ctime>
#include "GameQuadCells.h"
class State
{
private:

	char** ballsPositions = nullptr;
	State* parent = nullptr;
	bool Search(std::vector<State*>& v, State* what);
//	bool Repeats(std::vector<State*>& v);
	GameQuadCells gameQuadCells;
public:
	GameQuadCells& getGameQuadCellsNumber();
	void setGameQuadCellsNumber(GameQuadCells gqc);
	State* getParent() const;
	State(char** ballsPositions, GameQuadCells gameQuadCells = GameQuadCells());
	State(const State& state);
	State(State&& state) noexcept;
	State& operator=(const State& state);
	char** getBallsPositions() const;
	friend std::ostream& operator<<(std::ostream& out, const State& state);
	void Expand(std::vector<State*>& O, std::vector<State*>& C);
	void setParent(State* state);
	~State();
	bool operator==(const State& rhs) const;
	bool operator!=(const State& rhs) const;
};

#endif // !STATE_H
