#include "Ball.h"

Ball::Ball(float radius, sf::Color color)
	:radius(radius), m_frameTime(0), m_airForce(0), m_mass(0),
	m_totalTime(0), m_magnusForce(0), m_totalForce(0)
{
	this->shape = new sf::CircleShape(radius);
	this->shape->setFillColor(color);
	this->shape->setRadius(radius);
	this->shape->setOrigin(sf::Vector2f(radius, radius));
	this->dots = new sf::CircleShape[dotBufferCount];
	this->started = false;
	this->m_mode = ONLY_GRAVITY;
}

void Ball::shoot(sf::Vector2f startPos, float vel, float angle, REALISM mode)
{
	this->started = true;
	this->currentPos = startPos;
	this->xPos = 0;
	this->yPos = 0;
	this->resultingVelocity = vel;
	this->angle = angle;
	this->startVelocity.x = resultingVelocity * cosf(angle * (M_PI / 180));
	this->startVelocity.y = resultingVelocity * sinf(angle * (M_PI / 180));
	this->velocityX = startVelocity.x;
	this->velocityY = startVelocity.y;
	this->accelerationX = 0;
	this->accelerationY = -GRAVITY;

	this->m_mode = mode;


	sf::CircleShape dot(dotRadius);
	dot.setFillColor(sf::Color::Green);
	dot.setOrigin(sf::Vector2f(dotRadius, dotRadius));
	dot.setPosition(this->currentPos.x, currentPos.y);

	this->dots[dotIndex++] = dot;
	this->dotDelay == 0;

	this->m_mass = 2;

	switch (mode)
	{
	case ALL:
		this->dotColor = sf::Color::Magenta;
		break;
	case ONLY_GRAVITY:
		break;
	case AIR_RESISTANCE:
		this->dotColor = sf::Color(40, 108, 130);
		break;
	case MAGNUS_EFFECT:
		this->dotColor = sf::Color::Yellow;
		break;

	}
}

void Ball::update(float dt)
{
	if (started && !done)
	{
		dotDelay++;

		/*
			Tidsfaktorn är vår frame time: 16 ms, vilken vi även kan variera genom att höja/sänka vår FPS

			Starthastigeten är en godtycklig hastighet vi kan ge bollen.

			Startpositionen är där vi placerar musen när vi klickar, eller en godtycklig som ges när
			bollen

			I de inledande försöken är det enbart gravitationen g som verkar på bollen. Detta ger att
			vi får en konstant hastighet i x-led och en hastighet i y-led som beror av tidsfaktorn och 
			gravitationen. Hastigheten ges av formeln v = v0 + a*dt.

			Sträckan beror av den nuvarande positionen för objektet, dess utgångshastighet samt tidsfaktorn. 


			Luftmotståndskonstanten kommer att bli 0.29, då det är en kula. 

			Luftens densitet sätts, något godtyckligt, till 1,2041 kg/m3.

			Calculate: 
			
			Acceleration. Until we add forces, this will only be 
			gravity, basically. 

			Once we've added forces, we can calculate the acceleration through Fsum/mass

			The velocity based on previous frame
			New position

			Jämför mot: https://www.desmos.com/calculator/on4xzwtdwz

			save: 
			currentPos = newPos;
			currentVelocity = newVelocity;
		*/

		this->m_frameTime = (dt / 1000);
		this->m_totalTime += m_frameTime;

		this->shape->setPosition(this->currentPos);


		switch (this->m_mode)
		{
		case(ONLY_GRAVITY):
			break;
		case(AIR_RESISTANCE):
			this->doAir();
			break;
		case(MAGNUS_EFFECT):
			this->doMagnus();
			break;
		case(ALL):
			this->doAir();
			this->doMagnus();
			break;
		}

		this->m_totalForce = m_magnusForce + m_airForce;

		float forceX = m_totalForce * cos(angle);
		this->accelerationX = forceX / m_mass;

		float forceY = m_totalForce * sin(angle) - m_mass*GRAVITY;
		this->accelerationY = forceY / m_mass;

		this->velocityX += this->accelerationX * this->m_frameTime;
		this->velocityY += this->accelerationY * this->m_frameTime;

		this->startVelocity = sf::Vector2f(this->velocityX, this->velocityY);
		this->resultingVelocity = sqrt((this->startVelocity.x * this->startVelocity.x +
			this->startVelocity.y * this->startVelocity.y));

		this->deltaAngle = atanf(this->startVelocity.y / this->startVelocity.x) - angle;
		this->angle = atanf(this->startVelocity.y / this->startVelocity.x);

		this->currentPos.x = this->currentPos.x + this->velocityX * this->m_frameTime;
		this->currentPos.y = this->currentPos.y - this->velocityY * this->m_frameTime;

		//Set position before you calculate the new position, or else the " point of origin" 
		//won't ever be used, as we update the position before it is used.

		//Save current Values afor output;
		output_info info;
		info.posX = std::to_string(currentPos.x);
		info.posY = std::to_string(currentPos.y);
		info.velX = std::to_string(this->velocityX);
		info.velY = std::to_string(this->velocityY);
		info.timeSinceLauch = std::to_string(m_totalTime);
		this->projectile_info.push_back(info);


		if (dotDelay == dotDensity)
		{
			if (dotIndex == dotBufferCount)
				dotIndex = 0;

			sf::CircleShape dot(dotRadius);
			dot.setFillColor(this->dotColor);
			dot.setOrigin(sf::Vector2f(dotRadius, dotRadius));
			dot.setPosition(this->currentPos.x, currentPos.y);

			this->dots[dotIndex++] = dot;
			
			dotDelay = 0;
		}

	}
}

void Ball::doAir()
{
	//Negativ, då kraften kommer att vara riktad åt motsatt håll som vår boll färdas.
	//Vet inte riktigt vilken koeefficient vi borde ha här, men det är nog bara att välja en
	//så länge vi kan hänvisa till bra källor för det. Likadant med luftmotståndet. 
	this->m_airForce = -0.47 * ((1.2941 * M_PI * pow(radius * 0.01, 2) * pow(resultingVelocity, 2)) / 2);
}

void Ball::doMagnus()
{
	//Jag *tror* att den här beräknas på rätt sätt. 
	float angularVelocity = deltaAngle / m_frameTime;
	this->m_magnusForce = -(2*M_PI* 1.2941*resultingVelocity*pow(radius * 0.001, 2)*angularVelocity) / (2*(radius*0.01));
}

void Ball::setFinished()
{
	this->shape->setFillColor(sf::Color(0, 0, 0, 0));
	this->done = true;
}



void Ball::draw(sf::RenderTarget& target, sf::RenderStates states) const
{

	target.draw(*this->shape);
	for (int i = 0; i < dotBufferCount; i++)
	{
		target.draw(dots[i]);
	}

}

Ball::~Ball()
{
	if (this->shape != nullptr)
	{
		delete this->shape;
		this->shape = nullptr;
	}
	if (this->dots != nullptr)
	{
		delete[] this->dots;
		this->dots = nullptr;
	}
}
