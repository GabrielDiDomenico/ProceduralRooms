#ifndef __MATHAUX__H__
#define __MATHAUX__H__

#include <math.h>
#include <vector>

using namespace std;

class MathAux {

public:

	int static CalculateDistance(pair<int, int> pos1, pair<int, int> pos2) {
		return sqrt(pow(pos1.first - pos2.first, 2) + pow(pos1.second - pos2.second, 2));
	}

	float static CalculateInfluency(float distance, float effectArea) {
		if (distance > effectArea || effectArea <= 0) {
			return 0.0f;
		}
		return exp(-distance / (effectArea * 0.5f));
	}

	float static CalculateTreasureScore(float distWeight, float repulsionRadius, float repulsionWeight, pair<int, int> targetPos, pair<int, int> initPos, vector<pair<int, int>> treasurePosList) {
		float initDist = (float)CalculateDistance(targetPos, initPos);
		float score = initDist * distWeight;

		for (pair<int, int> td : treasurePosList) {
			float dist = (float)CalculateDistance(targetPos, td);

			score -= CalculateInfluency(dist, repulsionRadius) * repulsionWeight;
		}

		return score;
	}

	int static CalculateEnemyScore(int attractionWeight, int enemyAttractionRadius, int repulsionWeight, int enemyRepulsionRadius, pair<int, int> targetPos, vector<pair<int, int>> treasurePosList, vector<pair<int, int>> enemyPosList) {
		int score = 0;

		for (pair<int, int> td : treasurePosList) {
			int dist = CalculateDistance(targetPos, td);
			int attraction = CalculateInfluency(dist, enemyAttractionRadius) * attractionWeight;
			score += attraction;
		}

		for (pair<int, int> ed : enemyPosList) {
			int dist = CalculateDistance(targetPos, ed);
			int repulsion = CalculateInfluency(dist, enemyRepulsionRadius) * repulsionWeight;
			score -= repulsion;
		}

		return score;
	}

};

#endif