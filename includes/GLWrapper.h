/*

	Outracks GLWrapper
	Tiny OpenGL wrapping framework for demo use.

    You are free to redistribute, change and use this code without charge.
	The code is provided "as is", with no warranties or support.

 */

#pragma once

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <math.h>
#include <stdio.h>
#include "basswrap.h"


namespace GLWrapper
{
	LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

	class Window
	{
		HWND hWnd;
		HDC hDc;
		HGLRC hRC;
		WNDCLASSEX windowClass;
		char* CLASSNAME;
	public:
		Window(int width, int height, int colordepth, bool fullscreen, char* title);
		~Window();
		bool update();
		void BASS_replaySong(void);
		void BASS_playSong(char* fileName, float startTime = 0.0f);
		void BASS_changeSong(char *filename, float startTime = 0.0f);
		void BASS_getSyncData(float* buffer);
	};

	class Timer
	{
	public:
		Timer();
		~Timer();
		void restart();
		float getTime();
		float getG();
		float getInterval();
	private:
		LARGE_INTEGER currentTime;
		LARGE_INTEGER frequency;
		float factor;
		float startG;
		float g;
	};

	class RenderTarget
	{
	private:
		GLuint glhandle;
		int w, h, glwidth, glheight;
		float u, v;
		int getPow2(int value)
		{
			for (int i = 1; i < 32; i++) if (value <= (1<<i)) return (1<<i);
		}
		void create(int, int);
	public:
		int resolutionX;
		int resolutionY;
		RenderTarget(int width, int height);
		RenderTarget(int widht, int height, int resX, int resY);
		GLuint getGlHandle() { return glhandle; }
		GLuint getGlWidth() { return glwidth; }
		GLuint getGLHeight() { return glheight; }
		void enable() { glEnable(GL_TEXTURE_2D); glBindTexture(GL_TEXTURE_2D, glhandle); }
		void disable() { glDisable(GL_TEXTURE_2D); }
		void setFilterLinear();
		void setFilterNearest();
		void startRenderTo();
		void endRenderTo();
		void copyFramebuffer();
		void copyRenderTarget(RenderTarget& source);
		void blit();
		void blit(float red, float green, float blue, float alpha);
		void blitPositioned(float x, float y, float extraWidth, float extraHeight);
		void blitAdd() { blitAdd(1,1,1,1); }
		void blitAdd(float red, float green, float blue, float alpha);
		void blitMul() { blitMul(1,1,1,1); }
		void blitMul(float red, float green, float blue, float alpha);
		void blitReplace(float red, float green, float blue, float alpha);
		void blitTo(RenderTarget& target);
		void directionalBlur(int passes, int layers_pr_pass, float pixels_pr_layer, float alpha, float xv, float yv);
		void gaussianBlur(int passes, int layers_pr_pass, float pixels_pr_layer, float alpha);
	};

	class Texture
	{
		
		void loadFromFile(char* fileName);
	protected:
		Texture();
		int width;
		int height;
		bool fileTex;
	public:
		GLuint handle;
		Texture(char* fileName);
		Texture(int w, int h);
		~Texture();
		virtual void setWrapModeClamp();
		virtual void setWrapModeRepeat();
		virtual void enable();
		virtual void disable();
	};

	void EnableSEM();
	void DisableSEM();

	struct vertex
	{
		float x, y, z;
	};
	struct texcoord
	{
		float u, v;
	};
	struct triangle
	{
		int a, b, c;
	};
	vertex crossProd(vertex a, vertex b);
	vertex rotatex(vertex v, float theta);
	vertex rotatey(vertex v, float theta);
	vertex rotatez(vertex v, float theta);


	class Mesh
	{	
	public:
		int vertexCount;
		int triangleCount;
		vertex *vertexArray;
		vertex *normalArray;
		texcoord *texcoordArray;
		triangle *triangleArray;
		Mesh(int vtxCount, int triCount);
		~Mesh();
		void calcNormals();
		void draw();
		void offset(float x, float y, float z);
		void scale(float x, float y, float z);
	};

	class Model
	{
	public:
		int meshCount;
		Mesh **meshArray;
		Model(char* fileName);
		~Model();
		void calcNormals();
		void draw();
	};

	class Sound
	{
	public:
	};

}
