#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

#include "Simplex.h"
#include "Out.h"
// #include "user_data.h"
// #include "Plane.h"

Plane* Simplex::generate_plane(struct user_data::_userdata* ud) {
	int i, j, num_of_source_vars;
	user_data::_system::iterator it;
	eqparser::_vars::iterator eqit;
	Plane *p;

	numOfSourceVars = num_of_source_vars = ud->func.vars.size();
	old_plane = new Plane(num_of_source_vars);
	new_plane = p = new Plane(num_of_source_vars);

	for (i = 0, it = ud->system.begin(); it != ud->system.end(); it++, i++) {
		(*p->basisVars)[0][i] = num_of_source_vars + i + 1;
		(*p->basisVars)[1][i] = (*it)->rval;
	}

	for (i = 0, eqit = ud->func.vars.begin(); eqit != ud->func.vars.end(); eqit++, i++) {
		p->indexString[i] = (*eqit).a * -1;
		p->indexString[ud->func.vars.size() + i] = 0;
	}

	for (i = 0, it = ud->system.begin(); it != ud->system.end(); it++, i++) {
		for (j = 0, eqit = (*it)->vars.begin(); eqit != (*it)->vars.end(); eqit++, j++) {
			(*p->varsFactors)[i][j] = (*eqit).a;
			if ((*it)->sign == eqparser::ge)
				(*p->varsFactors)[i][j] *= -1;
			(*p->varsFactors)[i][(*it)->vars.size() + j] = j == i ? 1 : 0;
			if ((*it)->sign == eqparser::ge && (*p->varsFactors)[i][(*it)->vars.size() + j] != 0)
				(*p->varsFactors)[i][(*it)->vars.size() + j] *= -1;
		}
	}

	setIndexOfLeavingColumn(p);
	setThColumn(p);
	setIndexOfLeavingRow(p);
	setAllowingMember(p);

	return p;
}

void Simplex::run()
{
	int i = 1;
	Plane* t;
	enum result r;

	for (;;) {
		if (good_solution == (r = checkPlane(new_plane))) {
			dumpToTableTxt(new_plane, i, r);
			displayResult(new_plane, i, r);
			break;
		} else {
			if(!checkThColumn(new_plane)) {
				r = no_solution;
				dumpToTableTxt(new_plane, i, r);
				displayResult(new_plane, i, r);
				break;
			}
			dumpToTableTxt(new_plane, i, r);
		}

		++i;
		t = old_plane; old_plane = new_plane; new_plane = t;

		setTargetFunction(old_plane, new_plane);
		setBasisVars(old_plane, new_plane);
		setIndexString(old_plane, new_plane);
		setFactorsOfVars(old_plane, new_plane);
		setIndexOfLeavingColumn(new_plane);
		setThColumn(new_plane);
		setIndexOfLeavingRow(new_plane);
		setAllowingMember(new_plane);
	}	
}

bool Simplex::checkThColumn(Plane* p) {
	for (int i = 0; i < numOfSourceVars; ++i)
		if (p->thColumn[i] < 0)
			return false;
	return true;
}

enum result Simplex::checkPlane(Plane* p) {
	for (int i = 0; i < numOfSourceVars; ++i) {
		if (wayOfTargetFunction == eqparser::max) {
			if (p->indexString[i] < 0) return bad_solution;
		} else {
			if (p->indexString[i] >= 0) return bad_solution;
		}
	}
	return good_solution;
}

void Simplex::displayResult(Plane* p, unsigned int iteration, enum result r) {
	int i;
	std::stringstream out;

	if (r == good_solution) {
		_out << out << "Оптимальный план найден. Количество итераций = " << iteration << "\n\n";
		for (i = 0; i < numOfSourceVars; i++)
			out << "x" << (*p->basisVars)[0][i] << " = " << (*p->basisVars)[1][i] << "\n";
		out << "\nf(x) = " << p->targetFunction << "\n";
	}

	if (r == no_solution)
		_out << out << "Целевая функция не ограничена. Задача с данным условием не имеет решений.\n";

	std::cout << out.str() << std::flush;
}

void Simplex::dumpToTableTxt(Plane* p, unsigned int iteration, enum result r) {
	int i, j;
	static std::ofstream file("table.txt");
	std::stringstream buf;

//	buf << "Задана целевая функция: f(x) = ";	
//	for (i = 0; i < numOfSourceVars; ++i)
//	{
//		if (factorsOfTargetFunctionVars[i] > 0 && i)
//			buf << " + ";
//		buf << factorsOfTargetFunctionVars[i] << "x" <<  i + 1;
//	}

	buf << "\n\n";
	buf << iteration << "-й план \n\n";

	for (i = 0; i < numOfSourceVars; ++i) {
		buf << "x" << std::setw(3) << std::left << (*p->basisVars)[0][i] << "\t";
		buf << std::setw(5) << (*p->basisVars)[1][i] << "\t";
		for (j = 0; j < numOfSourceVars * 2; ++j)
			buf << std::setw(7) << (*p->varsFactors)[i][j] << "	";
		if (r == bad_solution)
			buf <<  p->thColumn [i];
		buf << "\n\n";
	}

	buf << "f(x)\t" << std::setw(5) << p->targetFunction << "\t";
	
	for (i = 0; i < numOfSourceVars * 2; ++i)
		buf << std::setw(7) << p->indexString[i] << "\t";
	
	buf << "\n\n";

	switch(r) {
	case bad_solution:
		buf << "Данный план не оптимален, его необходимо улучшить.";
		break;
	case good_solution:
		buf << "Данный план является оптимальным. Решение найдено.";
		break;
	case no_solution:
		buf << "Целевая функция не ограничена, данная задача не имеет решения.";
		break;
	}

	buf << "\n\n\n";

	file << buf.str() << std::flush;
}

void Simplex::setIndexOfLeavingColumn(Plane* p) { 
	int i;
	double minOfIndexString = p->indexString[0];
	p->indexOfLeavingColumn = 0;

	for (i = 0; i < numOfSourceVars * 2; ++i) {
		if (p->indexString[i] < 0 && minOfIndexString > p->indexString[i]) {
			minOfIndexString = p->indexString[i];
			p->indexOfLeavingColumn = i;
		}
	}
}

void Simplex::setIndexOfLeavingRow(Plane* p) {
	int i;
	double minOfThColumn = p->thColumn[0];
	p->indexOfLeavingRow = 0;

	for (i = 0; i < numOfSourceVars; ++i) {
		if (minOfThColumn > p->thColumn[i]) {
			minOfThColumn = p->thColumn[i];
			p->indexOfLeavingRow = i;
		}
	}
}

void Simplex::setAllowingMember(Plane* p) {
	p->allowingMember = (*p->varsFactors)[p->indexOfLeavingRow][p->indexOfLeavingColumn];
}

void Simplex::setBasisVars(Plane* source, Plane* target)
{
	int i;
	double A = (*source->basisVars)[1][source->indexOfLeavingRow];
	double B;

	for (i = 0; i < numOfSourceVars; ++i)
	{
		if (i == source->indexOfLeavingRow) {
			(*target->basisVars)[0][source->indexOfLeavingRow] = source->indexOfLeavingColumn + 1;
			(*target->basisVars)[1][i] = (*source->basisVars)[1][i] / source->allowingMember;
		} else {
			(*target->basisVars)[0][i] = (*source->basisVars)[0][i];
			B = (*source->varsFactors)[i][source->indexOfLeavingColumn];
			(*target->basisVars)[1][i] = (*source->basisVars)[1][i] - ((A * B) / source->allowingMember);
		}
	}
}

void Simplex::setFactorsOfVars(Plane* source, Plane* target) {
	int i, j;
	double A, B;

	for (i = 0; i < numOfSourceVars; ++i) {
		for (j = 0; j < numOfSourceVars * 2; ++j) {
			if (i == source->indexOfLeavingRow) {
				(*target->varsFactors)[i][j] = (*source->varsFactors)[i][j] / source->allowingMember;
			} else {
				A = (*source->varsFactors)[source->indexOfLeavingRow][j];
				B = (*source->varsFactors)[i][source->indexOfLeavingColumn];
				(*target->varsFactors)[i][j] = (*source->varsFactors)[i][j] - ((A * B) / source->allowingMember);
			}
		}
	}
}

void Simplex::setIndexString(Plane* source, Plane* target) {
		int i;
		double A, B;

		for (i = 0; i < numOfSourceVars * 2; ++i) {
			A = (*source->varsFactors)[source->indexOfLeavingRow][i];
			B = source->indexString[source->indexOfLeavingColumn];
			target->indexString[i] = source->indexString[i] - ((A * B) / source->allowingMember);
		}
}

void Simplex::setTargetFunction(Plane* source, Plane* target) {
		double A = (*source->basisVars)[1][source->indexOfLeavingRow];
		double B = source->indexString[source->indexOfLeavingColumn];
		target->targetFunction = source->targetFunction - ((A * B) / source->allowingMember);
}

void Simplex::setThColumn (Plane* p) {
	for (int i = 0; i < numOfSourceVars; ++i)
		p->thColumn[i] = (*p->basisVars)[1][i] / (*p->varsFactors)[i][p->indexOfLeavingColumn];
}
