

// class to represent the fuzz each instance of ArrayOf is a 'index' of fuzz
class ArrayOf : public mxObject
{
	Uint32 color; // its color
	bool explo; // internal variable to determine if in explode mode
	void logic(); // the 'index' 's logic private member function
public:
	ArrayOf(); // constructor , initalize variables
	void render(); // render to the screen the 'index'
	void setDirection ( int x, int y, int direction ); // set the index's direction
	void setColor ( Uint32 color ); // set the index's color
	int x_pos, y_pos,direction,color_r,color_g,color_b; // variables, to determine direction, and position, as well as  RGB values
	int which; // whether to set index by rgb value, or 32bit color value
};

ArrayOf::ArrayOf() // constructor
{
	mx = 0;
	which = 0;
}

void ArrayOf::setDirection ( int x, int y, int direction ) // set direction member function
{
	this->direction = direction;
	this->x_pos = x;
	this->y_pos = y;
}


void ArrayOf::render() // render to the screen
{
	logic();
	if ( mx != 0 )
		if ( which == 0 ) // set by rgb
			mx->setPixel ( mx->pscr,x_pos,y_pos,color_r,color_g,color_b );
		else // set by unsigned integer
			mx->setPixel ( mx->pscr,x_pos,y_pos,color );

}

void ArrayOf::logic() // logic of 'index'
{
	switch ( direction ) // what direction am I?
	{
		// move 'index' apporpiately by direction variable
		case 0: 
			x_pos += 2;
			y_pos -= 2;
			break;
		case 1:
			x_pos++;
			y_pos++;
			break;
		case 2:
			x_pos--;
			y_pos++;
			break;
		case 3:
			y_pos--;
			x_pos--;
			break;
		case 4:
			x_pos++;
			y_pos--;
			break;
		case 5:
			y_pos--;
			explo = true;
			break;
		case 6:
			y_pos++;
			explo = true;
			break;


	}

	// if explode is true
	static int wait = 0;
	if ( explo == true && ++wait > 25 )
	{
		direction = rand() %4;
		explo = false;
		wait = 0;
	}

	// is the 'index' out of bounds, if so, set it to a random direction
	if ( x_pos <= 0 || y_pos <= 0 || x_pos >= 640 || y_pos >= 480 )
	{
		setDirection ( rand() %640,rand() %480,rand() %4 );
		explo = false;
	}

}

// set the 'index's color by unsigned integer
void ArrayOf::setColor ( Uint32 color )
{
	this->color = color;
	which = 1; // sets which to 1
}

