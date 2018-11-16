#include "classifier.hpp"
#include <utility>
#include <vector>
#include <algorithm>

#include "digitVisualizer.hpp"
#include "data.hpp"


struct similarityAndLabel {
	float sim;
	int label;
};

Classifier::Classifier(std::vector<Digit> dataSet, const int k)
: c_dataSet(std::move(dataSet))
, c_simplifiedSize(c_dataSet.front().points().size())
, c_k(k)
{
}

float Classifier::similarity(const std::vector<cv::Point> firstPath, const std::vector<cv::Point2f> secondPath)
{
	float sumOfDistance = 0;
	for (int i = 0; i < firstPath.size(); i++)
	{
		sumOfDistance += cv::norm((cv::Point2f)firstPath[i] - secondPath[i]);
	}
	return sumOfDistance;
}

void Classifier::classify(const std::vector<cv::Point2f>& path)
{

	DataSet dataSet;

    // equidistant sampling
    simplify(path);

    // normalize and mirror y

	//CALCULATE THE BOUNDING BOX
	cv::Point2f topLeft = m_simplifiedPath[0];
	cv::Point2f btmRight = m_simplifiedPath[0];

	std::vector<cv::Point2f> normalizedPath(m_simplifiedPath);

	for (auto &i : m_simplifiedPath)
	{
		if (i.x <= topLeft.x)
		{
			topLeft.x = i.x;
		}
		if (i.y <= topLeft.y)
		{
			topLeft.y = i.y;
		}
		if (i.x >= btmRight.x)
		{
			btmRight.x = i.x;
		}
		if (i.y >= btmRight.y)
		{
			btmRight.y = i.y;
		}
	}

	//normalize that to 100x100 and mirror vertically
	for (auto &i : normalizedPath)
	{
		i.x = ((i.x - topLeft.x) / (btmRight.x - topLeft.x)) * 100;
		i.y = 100 - ((i.y - topLeft.y) / (btmRight.y - topLeft.y)) * 100;
	}

	//::vector<Digit> digits = dataSet.read("pendigits.tra");

	similarityAndLabel element;
	element.sim = 1000;
	element.label = -1;

	float maxSim = 0;
	int maxPosition = 0;

	std::vector<similarityAndLabel> knnVector(c_k,element);

	if (normalizedPath.size() == 8) 
	{
		for (auto &dataEntry : c_dataSet)
		{
			maxSim = 0;
			element.sim = similarity(dataEntry.points(), normalizedPath);
			element.label = dataEntry.label();
		
			for (int i = 0; i < knnVector.size(); i++) 
			{	
				if (knnVector[i].sim > maxSim)
				{
					maxSim = knnVector[i].sim;
					maxPosition = i;
				}
				
			}

			if (element.sim <= maxSim)
			{
				knnVector[maxPosition] = element;
			}

		}

	}
	else
	{
		std::cout << "not 8 elements";
	}

	std::vector<int>numberCounter (10, 0);
	for (int i = 0; i < c_k; i++)
	{
		numberCounter[knnVector[i].label]++;
	}

	int max = 0;
	for (int i = 0; i < numberCounter.size(); i++)
	{
		if (max < numberCounter[i])
		{
			max = numberCounter[i];
			maxPosition = i;
		}
	}

	m_result = maxPosition;

}

int Classifier::getResult() const
{
    return m_result;
}

std::vector<cv::Point2f> Classifier::getSimplifiedPath() const
{
    return m_simplifiedPath;
}

void Classifier::simplify(std::vector<cv::Point2f> path)
{

    // sample path with equal spacing and store in m_simplifiedPath
    // you should use c_simplifiedSize as number of points in simplified path
	m_simplifiedPath.clear();
	float length = 0;
	std::vector<cv::Vec2f> pathAsVector;
	cv::Point2f lastPoint = cv::Point2f(-1, -1);
	for (auto &i : path) 
	{
		if (lastPoint == cv::Point2f(-1, -1)) {
			lastPoint = i;
		}
		else
		{

			pathAsVector.push_back(cv::Vec2f((i - lastPoint ).x, (i - lastPoint).y));
			//std::cout << pathAsVector.back() << std::endl;
			lastPoint = i;
			length += cv::norm(pathAsVector.back());
		}

	}



	length = length / c_simplifiedSize;
	//std::cout << length << std::endl;
	double currentLength = length;
	cv::Vec2f currentVector = (0, 0);
	m_simplifiedPath.push_back(path[0]);
	//std::cout << "start: " <<  m_simplifiedPath.back() << std::endl;

	for (auto &i : pathAsVector)	
	{
		double normi = cv::norm(i);
		bool foundPoint = false;
		while (currentLength <= normi) 
		{
			m_simplifiedPath.push_back(path[0] + cv::Point2f(currentVector) + cv::Point2f(i/normi * currentLength));
			foundPoint = true;
			i = i - (i / normi * currentLength);
			currentLength = length;
			//std::cout << currentVector;
			//std::cout << m_simplifiedPath.back() << std::endl;
			normi = cv::norm(i);
		}
		if(!foundPoint) {
			currentLength -= normi;
		}
		currentVector += i;
	}
	if (m_simplifiedPath.size() < 8) {
		m_simplifiedPath.push_back(path.back());
	}
	//std::cout << "end: " <<  m_simplifiedPath.back() << std::endl;


	//std::cout << "Anzahl: " << m_simplifiedPath.size() << std::endl;

	//we got 8 points

	


    /*~~~~~~~~~~~*
     * YOUR CODE *
     * GOES HERE *
     *~~~~~~~~~~~*/
}
