

#include"mx.h" // include the old version of mx
#include<stdlib.h> // stdlib standard library include for rand and other functions
#include<time.h> // time.h for time
#include<string>
#include<unistd.h>
// function prototypes

void mix_in_image(); // for fade from one picture to the other
void shift_up(); 
void shift_down();
void update_screen ( int scr );
void onEvent ( SDL_Event *evnt );
void keyfunc ( int key );
void keyfuncup ( int key );
void mousemove ( int x, int y );
void mousedown ( int button, int x, int y );
void release_explosion();
void change_image();
void update_cords();
bool its_running = false;
int tx_pos, ty_pos,image_index = 0;
int amount = 640*480;
void init_joy();

#define screen_width 640*480

ArrayOf array_of_pixels[screen_width]; // the array of floating fuzz
mxHwnd mxhwnd; // old mx stuff
mxSprite logo;// old mx
SDL_Joystick *joy_stick[6]; // sdl structure for joystick data
int stick_axis[3]; // stick axis arrays



// main - entry point main function
int main ( int argc, char *argv[] )
{
	tx_pos = 0; 
	ty_pos = 0;

	// setup code

	if(argv[0][0] == '/')
	{
		std::string t = argv[0];
		std::string p =  t.substr(0, t.rfind("/"));
		chdir(p.c_str());
	}

	srand ( time ( 0 ) ); // seed randomization

	if ( mxhwnd.init ( "HPPD Simulation - www.lostsidedead.com",640,480,32,false ) == 0 )
	{
		logo.load ( &mxhwnd, "blackdots.bmp" );
		init_joy();
		for ( int z = 0; z <= rand() %4; z++ )
		{
			change_image(); // change the image
		}

		SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY,SDL_DEFAULT_REPEAT_INTERVAL);

		// old mx init stuff
		mxhwnd.setScreen ( 0 );
		mxhwnd.setEventHandler ( onEvent );
		mxhwnd.setKeydown ( keyfunc );
		mxhwnd.setKeyup ( keyfuncup );
		mxhwnd.setMouseDown ( mousedown );
		mxhwnd.setMouseMove ( mousemove );
		mxhwnd.initLoop ( update_screen );

		//clean up
		for ( int i = 0; i < SDL_NumJoysticks(); i++ )
			SDL_JoystickClose ( joy_stick[i] );

	}
	return 0;

}

// init the joystick
void init_joy()
{
	if ( SDL_NumJoysticks() == 0 ) // joysticks not attached
	{
		printf ( "Joystick not found...\n" );
		return;
	}
	else
	{

		for ( int i = 0; i < SDL_NumJoysticks(); i++ )
			joy_stick[i] = SDL_JoystickOpen ( i );


		SDL_JoystickEventState ( SDL_ENABLE ); // enable joystick state

	}

}

/*

	Heres where the real meat of the program is. The render function


*/
void update_screen ( int scr )
{

	logo.display ( 0,0 ); // display the background image, the first layer on the plane of 'vision' 
	// the fuzz with be placed ontop of this , as another layer that is based from the background,
	// when the image mixing function is called (space) but is not the background and at start
	// the fuzz is set to random using rand seeded by srand.

	static bool init_ = false;
	if ( init_ == false )
	{
		for ( int i = 0; i < 640*480; i++ )
		{
			array_of_pixels[i].init ( &mxhwnd );
			array_of_pixels[i].color_r = rand() %255; // each Red value a random number
			array_of_pixels[i].color_g = rand() %255;//  each Green value a random number
			array_of_pixels[i].color_b = rand() %255; // each Blue Value a random number
			array_of_pixels[i].setDirection ( rand() %640,rand() %480,rand() %4 ); // Set the direction randomly
		}

		init_ = true; // init set to true, this will only execute once at the first called to update_screen
	}
	if ( its_running == true )
	{

		

		static int rr = rand() %255, rg = rand() %255, rb = rand() %255;

		// note the amount is the amount of distortion
		for ( int i = 0; i < 640*480; i++ )
		{
			if( (rand()%amount)%640*480 ==0 ) continue;
			// while the program is running we are to check each pixel 
			// if it goes out of bounds we randomize it back to the pool.
			if ( array_of_pixels[i].x_pos >= tx_pos && array_of_pixels[i].x_pos <= tx_pos+25 && array_of_pixels[i].y_pos >= ty_pos && array_of_pixels[i].y_pos <= ty_pos+25 )
			{
				array_of_pixels[i].color_r = rr;
				array_of_pixels[i].color_g = rg;
				array_of_pixels[i].color_b = rb;
				array_of_pixels[i].setDirection ( tx_pos,ty_pos,rand() %5 ); // set its position in a random direction
			}

			array_of_pixels[i].render(); // render the pixel
		}

		rr = rand() %255, rg = rand() %255, rb = rand() %255;
	}


	update_cords(); // grab input 
	SDL_UpdateRect ( mxhwnd.pscr,0,0,640,480 ); // update the screen

}

void update_cords()
{


	Uint8 *keys = SDL_GetKeyState(0);


	if(keys[SDLK_a])
		if(amount > 0) amount -= 100;
	

	if(keys[SDLK_s])
		if(amount < 640*480) amount += 100;



	if ( stick_axis[0] > 100 )
		tx_pos+=5;
	if ( stick_axis[0] < -100 )
		tx_pos-=5;
	if ( stick_axis[1] > 100 )
		ty_pos+=5;
	if ( stick_axis[1] < -100 )
		ty_pos-=5;

	for ( int z = 0; z < SDL_NumJoysticks(); z++ )
	{

		if ( SDL_JoystickGetButton ( joy_stick[z],5 ) )
		{
			change_image();
		}

		if ( SDL_JoystickGetButton ( joy_stick[z],0 ) )
		{
			shift_up();
		}

		if ( SDL_JoystickGetButton ( joy_stick[z],1 ) )
		{
			shift_down();
		}
		if ( SDL_JoystickGetButton ( joy_stick[z],2 ) )
		{
			release_explosion();
			return;
		}
		if ( SDL_JoystickGetButton ( joy_stick[z],3 ) )
		{
			mix_in_image();
		}
		if ( SDL_JoystickGetButton ( joy_stick[z],9 ) )
		{
			its_running = true;
		}
	}

	SDL_UpdateRect ( mxhwnd.pscr,0,0,640,480 );
}

void onEvent ( SDL_Event *evnt )
{

	switch ( evnt->type )
	{
		case SDL_JOYAXISMOTION:
			if ( evnt->jaxis.axis < 2 )
				stick_axis[evnt->jaxis.axis]=evnt->jaxis.value>>8;
			update_cords();
			break;
	}
}

void mix_in_image()
{
	int pos = 0;
	for ( int i = 0; i < 640; i++ )
	{
		for ( int z = 0; z < 480; z++ )
		{
			// for every pixel, grab the pixel from the background pixel
			// add it to the pool and set it in a random direction
			// note the amount seen is affected by the amount variable
			Uint32 color = mxhwnd.getPixel ( mxhwnd.pscr,i,z );
			array_of_pixels[pos].setDirection ( i,z,rand() %6 );
			array_of_pixels[pos].setColor ( color );
			pos++;
		}
	}
}

// Key proccessing
void keyfunc ( int key )
{

	printf ( "%d", key );


	switch ( key )
	{
		case 13: // start the distortion
			release_explosion();
			printf ( "Released..\n" );
			break;
		case 273:
			shift_up(); 
			break;
		case 274:
			shift_down(); 
			break;
		case 32:
			mix_in_image();
			break;
		case 27:
			mxhwnd.kill();// exit
			break;
		case 275:
		case 276:
			change_image();// switch background images for distortion image blend.
			break;
		
	}

}
void keyfuncup ( int key )
{

}
void mousemove ( int x, int y )
{
	tx_pos = x;
	ty_pos = y;

}
void mousedown ( int button, int x, int y )
{
	// effects when you click the mouse
	switch ( button )
	{
		case 1:
			for ( int i = 0; i < 640*480;i++ )
			{
				array_of_pixels[i].setDirection ( array_of_pixels[i].x_pos, array_of_pixels[i].y_pos, rand() %6 );
			}
			break;
		case 2:
			for ( int i = 0; i < 640*480;i++ )
			{
				array_of_pixels[i].setDirection ( array_of_pixels[i].x_pos, array_of_pixels[i].y_pos, rand() %2 >= 1 ? 5 : 6 );
			}
			break;

	}

}

void release_explosion()
{
	// start the fuzz
	its_running = true;
}

void shift_up()
{
	// shift some up the pixels directions ( just to make it seem as if it is a pattern since it is)
	for ( int i = 0; i < 640*480; i++ )
	{
		if ( array_of_pixels[i].x_pos >= 0 && array_of_pixels[i].x_pos <= 640 && array_of_pixels[i].y_pos >= 440 && array_of_pixels[i].y_pos <= 480 )
		{
			array_of_pixels[i].setDirection ( array_of_pixels[i].x_pos,array_of_pixels[i].y_pos,rand() %6 );
		}
	}
}

void shift_down()
{

	// shift the the top pixels and set them in random directions
	for ( int i = 0; i < 640*480; i++ )
	{
		if ( array_of_pixels[i].x_pos >= 0 && array_of_pixels[i].x_pos <= 640 && array_of_pixels[i].y_pos >= 0 && array_of_pixels[i].y_pos <= 40 )
		{
			array_of_pixels[i].setDirection ( array_of_pixels[i].x_pos,array_of_pixels[i].y_pos,rand() %6 );
		}
	}

}
// change the basis image (background)
void change_image()
{
	image_index++;
	if ( image_index >= 6 )
		image_index = 0;

	// bad practice I know I know I was young (er)
	switch ( image_index )
	{
		case 1:
			logo.~mxSprite();
			logo.load ( "logo2.bmp" ); 
			logo.display ( 0,0 );
			SDL_UpdateRect ( mxhwnd.pscr,0,0,640,480 );

			break;
		case 2:
			logo.~mxSprite();
			logo.load ( "logo3.bmp" );
			logo.display ( 0,0 );
			SDL_UpdateRect ( mxhwnd.pscr,0,0,640,480 );

			break;
		case 3:
			logo.~mxSprite();
			logo.load ( "logo4.bmp" );
			logo.display ( 0,0 );
			SDL_UpdateRect ( mxhwnd.pscr,0,0,640,480 );

			break;
		case 4:
			logo.~mxSprite();
			logo.load ( "blackdots.bmp" );
			logo.display ( 0,0 );
			SDL_UpdateRect ( mxhwnd.pscr,0,0,640,480 );
			break;
		case 5:
			logo.~mxSprite();
			logo.load ( "logo1.bmp" );
			logo.display ( 0,0 );
			SDL_UpdateRect ( mxhwnd.pscr,0,0,640,480 );
			break;
	}
}
