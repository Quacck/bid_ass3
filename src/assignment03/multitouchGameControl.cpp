#include "multitouchGameControl.hpp"

MultitouchGameControl::MultitouchGameControl(Game& game, HomograpyCalibrator& calibrator) : m_game(game), m_calibrator(calibrator)
{
}

void MultitouchGameControl::handleInput(std::vector<cv::RotatedRect> positions)
{
	//check wether a positions is in any paddle's bounding box
	std::cout << "hallo bin in handleInput" << std::endl;

	for (auto &currentPlayer : m_game.players()) {
		for (auto &currentPaddle : currentPlayer.paddles()) {
			for (auto &currentPosition : positions) 
			{
				if (currentPaddle.boundingBox().contains(currentPosition.center)) {
					currentPaddle.setActive(true);
				}
				else
				{
					if (currentPaddle.active())
					{
						if (currentPaddle.boundingBox().tl().x <= currentPosition.center.x <= currentPaddle.boundingBox().br().x)
						{
							currentPaddle.setTargetPosition(currentPosition.center.y);
						}

						currentPaddle.setActive(false);
					}
				}

			}
		}
	}
	std::cout << "hallo leave handleInput" << std::endl;

    /*~~~~~~~~~~~*
     * YOUR CODE *
     * GOES HERE *
     *~~~~~~~~~~~*/

    // Use m_calibrator to access and modify the m_game's player's pedals.
    // How you do it is your call, but you should probably use active(), setActive(), position() and setTargetPosition().
}
