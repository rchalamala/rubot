#include <array>
#include <cassert>
#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <vector>
//#include "color_recognition.hpp"

class Cube
{
	enum COLORS
	{
		BLUE = 0, YELLOW = 1, GREEN = 2, ORANGE = 3, RED = 4, WHITE = 5
	};

	std::map<std::pair<std::size_t, std::size_t>, std::pair<std::size_t, std::size_t>> edges{ {{BLUE,   1}, {WHITE,  1}},
																							 {{BLUE,   3}, {RED,    4}},
																							 {{BLUE,   4}, {ORANGE, 3}},
																							 {{BLUE,   6}, {YELLOW, 6}},

																							 {{YELLOW, 1}, {GREEN,  6}},
																							 {{YELLOW, 3}, {ORANGE, 6}},
																							 {{YELLOW, 4}, {RED,    6}},
																							 {{YELLOW, 6}, {GREEN,  6}},

																							 {{GREEN,  1}, {WHITE,  6}},
																							 {{GREEN,  3}, {ORANGE, 4}},
																							 {{GREEN,  4}, {RED,    3}},
																							 {{GREEN,  6}, {YELLOW, 1}},

																							 {{ORANGE, 1}, {WHITE,  3}},
																							 {{ORANGE, 3}, {BLUE,   4}},
																							 {{ORANGE, 4}, {YELLOW, 3}},
																							 {{ORANGE, 6}, {GREEN,  3}},

																							 {{RED,    1}, {WHITE,  4}},
																							 {{RED,    3}, {GREEN,  4}},
																							 {{RED,    4}, {BLUE,   3}},
																							 {{RED,    6}, {YELLOW, 4}},

																							 {{WHITE,  1}, {BLUE,   1}},
																							 {{WHITE,  3}, {ORANGE, 1}},
																							 {{WHITE,  4}, {GREEN,  1}},
																							 {{WHITE,  6}, {RED,    1}},

	};

	std::array<std::array<char, 8>, 6> cube;
	std::array<char, 6> base_moves{ 'B',
								   'D',
								   'F',
								   'L',
								   'R',
								   'U' }, color_characters{ 'B',
														  'Y',
														  'G',
														  'O',
														  'R',
														  'W' };
	std::vector<std::function<void(bool)>> moves;
	std::string solution;

	bool is_solved()
	{
		for (std::size_t color = 0; color < 6; ++color)
		{
			for (std::size_t piece = 0; piece < 8; ++piece)
			{
				if (cube[color][piece] != color_characters[color])
				{
					return false;
				}
			}
		}
		return true;
	}

	void base_cube()
	{
		for (std::size_t color_index = 0; color_index < 6; ++color_index)
		{
			for (std::size_t piece = 0; piece < 8; ++piece)
			{
				cube[color_index][piece] = color_characters[color_index];
			}
		}
	}

	static void swap_four(char& piece_0, char& piece_1, char& piece_2, char& piece_3, const bool& clockwise)
	{
		if (clockwise)
		{
			std::swap(piece_0, piece_1);
			std::swap(piece_1, piece_2);
			std::swap(piece_2, piece_3);
		}
		else
		{
			std::swap(piece_0, piece_3);
			std::swap(piece_3, piece_2);
			std::swap(piece_2, piece_1);
		}
	}

	static void rotate(std::array<char, 8>& face, const bool& clockwise)
	{
		swap_four(face[0], face[5], face[7], face[2], clockwise);
		swap_four(face[1], face[3], face[6], face[4], clockwise);
	}

public:
	Cube()
	{
		base_cube();
		moves.emplace_back([&](const bool& clockwise) -> void
			{
				rotate(cube[BLUE], clockwise);
				swap_four(cube[WHITE][2], cube[RED][7], cube[YELLOW][5], cube[ORANGE][0], clockwise);
				swap_four(cube[WHITE][1], cube[RED][4], cube[YELLOW][6], cube[ORANGE][3], clockwise);
				swap_four(cube[WHITE][0], cube[RED][2], cube[YELLOW][7], cube[ORANGE][5], clockwise);
			});
		moves.emplace_back([&](const bool& clockwise) -> void
			{
				rotate(cube[YELLOW], clockwise);
				swap_four(cube[GREEN][5], cube[ORANGE][5], cube[BLUE][5], cube[RED][5], clockwise);
				swap_four(cube[GREEN][6], cube[ORANGE][6], cube[BLUE][6], cube[RED][6], clockwise);
				swap_four(cube[GREEN][7], cube[ORANGE][7], cube[BLUE][7], cube[RED][7], clockwise);
			});
		moves.emplace_back([&](const bool& clockwise) -> void
			{
				rotate(cube[GREEN], clockwise);
				swap_four(cube[WHITE][5], cube[ORANGE][7], cube[YELLOW][2], cube[RED][0], clockwise);
				swap_four(cube[WHITE][6], cube[ORANGE][4], cube[YELLOW][1], cube[RED][3], clockwise);
				swap_four(cube[WHITE][7], cube[ORANGE][2], cube[YELLOW][0], cube[RED][5], clockwise);
			});
		moves.emplace_back([&](const bool& clockwise) -> void
			{
				rotate(cube[ORANGE], clockwise);
				swap_four(cube[WHITE][0], cube[BLUE][7], cube[YELLOW][0], cube[GREEN][0], clockwise);
				swap_four(cube[WHITE][3], cube[BLUE][4], cube[YELLOW][3], cube[GREEN][3], clockwise);
				swap_four(cube[WHITE][5], cube[BLUE][2], cube[YELLOW][5], cube[GREEN][5], clockwise);
			});
		moves.emplace_back([&](const bool& clockwise) -> void
			{
				rotate(cube[RED], clockwise);
				swap_four(cube[WHITE][7], cube[GREEN][7], cube[YELLOW][7], cube[BLUE][0], clockwise);
				swap_four(cube[WHITE][4], cube[GREEN][4], cube[YELLOW][4], cube[BLUE][3], clockwise);
				swap_four(cube[WHITE][2], cube[GREEN][2], cube[YELLOW][2], cube[BLUE][5], clockwise);
			});
		moves.emplace_back([&](const bool& clockwise) -> void
			{
				rotate(cube[WHITE], clockwise);
				swap_four(cube[BLUE][2], cube[ORANGE][2], cube[GREEN][2], cube[RED][2], clockwise);
				swap_four(cube[BLUE][1], cube[ORANGE][1], cube[GREEN][1], cube[RED][1], clockwise);
				swap_four(cube[BLUE][0], cube[ORANGE][0], cube[GREEN][0], cube[RED][0], clockwise);
			});
	}

	void turn(const std::string& move)
	{
		assert(!move.empty() && move.size() <= 2);
		assert(move.front() == 'B' || move.front() == 'D' || move.front() == 'F' || move.front() == 'L' || move.front() == 'R' || move.front() == 'U');
		assert(move.size() == 1 || move.back() == '\'' || move.back() == '2');
		for (std::size_t base_move = 0; base_move < 6; ++base_move)
		{
			if (move.front() == base_moves[base_move])
			{
				moves[base_move](move.back() != '\'');
				if (move.back() == '2')
				{
					moves[base_move](true);
				}
				break;
			}
		}
	}

	void print()
	{
		for (std::size_t color = 0; color < 6; ++color)
		{
			for (std::size_t piece = 0; piece < 8; ++piece)
			{
				std::cout << cube[color][piece] << (piece != 2 && piece != 4 ? ' ' : '\n');
				if (piece == 3)
				{
					std::cout << color_characters[color] << ' ';
				}
			}
			std::cout << "\n\n";
		}
	}

	std::string get_solution()
	{
		return solution;
	}

	void complete_search(std::size_t base_move = 0, const std::size_t& moves_performed = 0)
	{
		if (is_solved() || moves_performed == 20)
		{
			return;
		}
		for (; base_move < 6; ++base_move)
		{
			solution += base_moves[base_move];
			turn(std::string(1, base_moves[base_move]));
			complete_search((base_move + 1) % 6, moves_performed + 1);
			if (is_solved())
			{
				return;
			}
			solution += '2';
			turn(std::string(1, base_moves[base_move]));
			complete_search((base_move + 1) % 6, moves_performed + 1);
			if (is_solved())
			{
				return;
			}
			solution.pop_back();
			solution += '\'';
			turn(std::string(1, base_moves[base_move]));
			complete_search((base_move + 1) % 6, moves_performed + 1);
			if (is_solved())
			{
				return;
			}
			solution.pop_back();
			turn(std::string(1, base_moves[base_move]));
		}
	}

	void set_color(const char& color, const std::array<char, 8>& set_colors)
	{
		for (std::size_t color_index = 0; color_index < 6; ++color_index)
		{
			if (color == color_characters[color_index])
			{
				for (std::size_t piece = 0; piece < 8; ++piece)
				{
					cube[color_index][piece] = set_colors[piece];
				}
				break;
			}
		}
	}
};

int main()
{
	Cube cube{};
	/*
	cube.set_color('B', );
	cube.set_color('Y', );
	cube.set_color('G', );
	cube.set_color('O', );
	cube.set_color('R', );
	cube.set_color('W', );
	*/
	//test1();
	cube.turn("B'");
	cube.turn("F2");
	cube.turn("R2");
	cube.turn("U'");
	cube.turn("B2");
	cube.turn("F2");
	cube.turn("U");
	cube.turn("R'");
	cube.turn("L2");
	cube.turn("F'");
	cube.print();
	//cube.complete_search();
	//std::cout << cube.get_solution() << '\n';
}
