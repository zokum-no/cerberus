#ifndef LOADINGSCREENH
#define LOADINGSCREENH

typedef struct {
	int entries;
	int progress;

	float xstart;
	float xend;

	float ystart;
	float yend;

	float width;
	float barlength;

	char *title;
	char *subtitle;
} progresscounter;

void loadingscreen_fadeout(DWORD progress, Texture &pic);

int loadingscreen(Window &wnd, progresscounter *, Texture &bgpic);

progresscounter *newcounter(float xstart, float ystart, float xend, float yend, float width, int entries, char *, char*);
void progressbar(progresscounter *);


#endif