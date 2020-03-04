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
}

void Ball::shoot(sf::Vector2f startPos, float vel, float angle)
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


	sf::CircleShape dot(dotRadius);
	dot.setFillColor(sf::Color::Green);
	dot.setOrigin(sf::Vector2f(dotRadius, dotRadius));
	dot.setPosition(this->currentPos.x, currentPos.y);

	this->dots[dotIndex++] = dot;

	this->m_mass = 2;
}

void Ball::update(float dt, sf::Vector2f cursorPos )
{
	static int dotDelay = 0;

	if (started)
	{
		dotDelay++;

		/*
			Tidsfaktorn �r v�r frame time: 16 ms, vilken vi �ven kan variera genom att h�ja/s�nka v�r FPS

			Starthastigeten �r en godtycklig hastighet vi kan ge bollen.

			Startpositionen �r d�r vi placerar musen n�r vi klickar, eller en godtycklig som ges n�r
			bollen

			I de inledande f�rs�ken �r det enbart gravitationen g som verkar p� bollen. Detta ger att
			vi f�r en konstant hastighet i x-led och en hastighet i y-led som beror av tidsfaktorn och 
			gravitationen. Hastigheten ges av formeln v = v0 + a*dt.

			Str�ckan beror av den nuvarande positionen f�r objektet, dess utg�ngshastighet samt tidsfaktorn. 


			Luftmotst�ndskonstanten kommer att bli 0.29, d� det �r en kula. 

			Luftens densitet s�tts, n�got godtyckligt, till 1,2041 kg/m3.

			Calculate: 
			
			Acceleration. Until we add forces, this will only be 
			gravity, basically. 

			Once we've added forces, we can calculate the acceleration through Fsum/mass

			The velocity based on previous frame
			New position

			J�mf�r mot: https://www.desmos.com/calculator/on4xzwtdwz

			save: 
			currentPos = newPos;
			currentVelocity = newVelocity;
		*/

		this->m_frameTime = (dt / 1000);
		this->m_totalTime += m_frameTime;

		this->shape->setPosition(this->currentPos);

		//f�renkla formeln, men h�ll den separat fr�n magnus. Jag �r inte helt hundra p� det h�r �n. 
		//som sagt, den �r en aning bork'd atm-
		this->doAir();
		this->doMagnus();
		this->m_totalForce = -m_magnusForce + m_airForce;

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

		if (dotDelay == dotDensity)
		{
			if (dotIndex == dotBufferCount)
				dotIndex = 0;

			sf::CircleShape dot(dotRadius);
			dot.setFillColor(sf::Color::Green);
			dot.setOrigin(sf::Vector2f(dotRadius, dotRadius));
			dot.setPosition(this->currentPos.x, currentPos.y);

			this->dots[dotIndex++] = dot;
			
			dotDelay = 0;
		}

	}
	else//Follows cursor Position
	{
		this->shape->setPosition((float)cursorPos.x, (float)cursorPos.y);
	}

}

void Ball::doAir()
{
	//Negativ, d� kraften kommer att vara riktad �t motsatt h�ll som v�r boll f�rdas.
	//Vet inte riktigt vilken koeefficient vi borde ha h�r, men det �r nog bara att v�lja en
	//s� l�nge vi kan h�nvisa till bra k�llor f�r det. Likadant med luftmotst�ndet. 
	this->m_airForce = -0.47 * ((1.2941 * M_PI * pow(radius * 0.01, 2) * pow(resultingVelocity, 2)) / 2);
	//float airX = this->m_airForce * cosf(angle);
	//this->accelerationX = airX / this->m_mass;

	//float airY = this->m_airForce * sinf(angle) - this->m_mass * GRAVITY;
	//this->accelerationY = (airY / m_mass);
}

void Ball::doMagnus()
{
	float angularVelocity = deltaAngle / m_frameTime;
	this->m_magnusForce = (2*M_PI* 1.2941*resultingVelocity*pow(radius * 0.001, 2)*angularVelocity) / (2*(radius*0.01));
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
