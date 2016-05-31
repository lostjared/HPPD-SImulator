
/*
 *
 * HPPD-Light
 *
 * The Pixel's of Light Pull on Each Other
 * Its actually quite simple
 * I am getting Closer
 *
 * this application uses mx
 * to get mx
 *
 * $ svn co svn://lostsidedead.com/haze/trunk/libmx
 *
 * -Jared
 *
 */

#include<mx/mx.h>
#include<mx/mxjpeg.h>

class lightWindow : public mx::mxWnd {
	size_t i,z;
public:
	lightWindow() : mxWnd(1024, 768, 0, 0) {
		mx::mxJpeg jpgloader;
		jpgloader.jpgOpen("img/img.jpg");
		surface = jpgloader.LoadJPG();
		jpgloader.jpgClose();
		setTitle("HPPD-Light - [ Press Up to Increase, Down to Decrease ]");
		constant = 3;
	}

	virtual void eventPassed(SDL_Event &e) {
		mxWnd::eventPassed(e);

		if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_RETURN)
			SDL_SaveBMP(front, "ss.bmp");

		if(e.type == SDL_KEYDOWN)
			switch(e.key.keysym.sym) {
			case SDLK_UP:
				if(constant < 10)
					++constant;
				break;
			case SDLK_DOWN:
				if(constant > 2)
					--constant;
				break;
			}

	}

	virtual void renderScreen() {
		surface.lockSurface();
		for(i = 10; i < front.width()-10; i++) {
			for(z = 10; z < front.height()-10; z++) {
				surface[(i-rand()%constant)+(z+rand()%constant)*surface.width()] = surface[(i+rand()%constant)+(z-rand()%constant)*surface.width()];
				surface[(i+rand()%constant)+(z-rand()%constant)*surface.width()] = surface[(i-rand()%constant)+(z+rand()%constant)*surface.width()];
			}
		}

		surface.unlockSurface();
		front.copySurface(surface, 0, 0);
		front.Flip();
	}
private:
	mx::mxSurface surface;
	int constant;
};


int main(int argc, char **argv) {

	try {
		lightWindow light;
		return light.messageLoop();
	}
	catch(mx::mxException<std::string> &e) {
		e.printError(std::cerr);
	}
	catch(std::exception &e) {
		std::cerr << e.what() << "\n";
	}
	catch(...) {
		std::cerr << "unknown error\n";
	}
	return EXIT_FAILURE;
}
