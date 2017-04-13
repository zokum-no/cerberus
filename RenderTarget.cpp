#include "includes/GLWrapper.h"

namespace GLWrapper
{
	RenderTarget::RenderTarget(int width, int height)
	{
		resolutionX = 800;
		resolutionY = 600;
		create(width, height);
	}

	RenderTarget::RenderTarget(int width, int height, int resX, int resY)
	{
		resolutionX = resX;
		resolutionY = resY;
		create(width, height);
	}

	void RenderTarget::create(int width, int height)
	{
		w = width;
		glwidth = getPow2(width);
		h = height;
		glheight = getPow2(height);
		glGenTextures(1, &glhandle);
		glBindTexture(GL_TEXTURE_2D, glhandle);
		char* data = new char[glwidth*glheight*4];
		for (int i = 0; i < glwidth*glheight*4; i++) data[i] = 0;
		glTexImage2D(GL_TEXTURE_2D, 0, 4, glwidth, glheight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		u = float(width) / float(glwidth);
		v = float(height) / float(glheight);
	}

	void RenderTarget::setFilterLinear()
	{
		glBindTexture(GL_TEXTURE_2D, glhandle);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	void RenderTarget::setFilterNearest()
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}

	void RenderTarget::startRenderTo()
	{
		glViewport(0,0,w,h);
	}

	void RenderTarget::endRenderTo()
	{
		glBindTexture(GL_TEXTURE_2D, glhandle);
		glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, w, h);
		glViewport(0, 0, resolutionX, resolutionY);
	}

	void RenderTarget::copyFramebuffer()
	{
		glBindTexture(GL_TEXTURE_2D, glhandle);
		glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, w, h);
	}

	void RenderTarget::copyRenderTarget(RenderTarget& source)
	{
		startRenderTo();
		source.blit();
		endRenderTo();
	}

	void RenderTarget::blitPositioned(float x, float y, float extraWidth, float extraHeight)
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, glhandle);
		
		glDisable(GL_DEPTH_TEST);
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();

		float X = (x / resolutionX) * 2.0f - 1.0f;
		float Y = (y / resolutionY) * 2.0f - 1.0f;
		float Xex = (extraWidth / resolutionX) * 2.0f;
		float Yex = (extraHeight / resolutionY) * 2.0f;

		glBegin(GL_QUADS);
		glTexCoord2f(0,0); glVertex3f(X,Y,0);
		glTexCoord2f(u,0); glVertex3f(X+2+Xex,Y,0);
		glTexCoord2f(u,v); glVertex3f(X+2+Xex,Y+2+Yex,0);
		glTexCoord2f(0,v); glVertex3f(X,Y+2+Yex,0);
		glEnd();

		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glEnable(GL_DEPTH_TEST);
	}

	void RenderTarget::blit() 
	{ 
		blitPositioned(0,0,0,0); 
	}

	void RenderTarget::blit(float red, float green, float blue, float alpha)
	{
		glColor4f(red, green, blue, alpha);
		blitPositioned(0,0,0,0);
	}

	void RenderTarget::blitAdd(float r, float g, float b, float a)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		glColor4f(r,g,b,a);
		blit();
		glColor4f(1,1,1,1);
		glDisable(GL_BLEND);
	}

	void RenderTarget::blitReplace(float r, float g, float b, float a)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColor4f(r,g,b,a);
		blit();
		glColor4f(1,1,1,1);
		glDisable(GL_BLEND);
	}

	void RenderTarget::blitMul(float r, float g, float b, float a)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_DST_COLOR, GL_ZERO);
		glColor4f(r,g,b,a);
		blit();
		glColor4f(1,1,1,1);
		glDisable(GL_BLEND);
	}

	void RenderTarget::blitTo(RenderTarget& target)
	{
		target.startRenderTo();
		glDisable(GL_BLEND);
		blit();
		target.endRenderTo();
	}

	void RenderTarget::gaussianBlur(int passes, int layers_pr_pass, float pixels_pr_layer, float alpha)
	{
		directionalBlur(passes, layers_pr_pass, pixels_pr_layer, alpha, 1.0f, 0.0f);
		directionalBlur(passes, layers_pr_pass, pixels_pr_layer, alpha, 0.0f, 1.0f);
	}

	void RenderTarget::directionalBlur(int passes, int layers_pr_pass, float pixels_pr_layer, float alpha, float xv, float yv)
	{
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();

		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		glColor4f(1.0f, 1.0f, 1.0f, alpha / (float(layers_pr_pass*2)+1));
		glBindTexture(GL_TEXTURE_2D, glhandle);
		glEnable(GL_TEXTURE_2D);
		float ofs = 2.0f*pixels_pr_layer / w;
		for (int n = 1; n < passes+1; n++)
		{
			startRenderTo();
			glClearColor(0,0,0,1);
			glClear(GL_COLOR_BUFFER_BIT);		
			for (int i = -layers_pr_pass; i < layers_pr_pass+1; i++)
			{
				glBegin(GL_QUADS);
				glTexCoord2f(0,0);	glVertex3f(-1.0f+ofs*n*i*xv,	-1.0f+ofs*n*i*yv, 0);
				glTexCoord2f(u,0);	glVertex3f( 1.0f+ofs*n*i*xv,	-1.0f+ofs*n*i*yv, 0);
				glTexCoord2f(u,v);	glVertex3f( 1.0f+ofs*n*i*xv,     1.0f+ofs*n*i*yv, 0);
				glTexCoord2f(0,v);	glVertex3f(-1.0f+ofs*n*i*xv,	 1.0f+ofs*n*i*yv, 0);
				glEnd();
			}
			endRenderTo();
		}

		glDisable(GL_BLEND);
		glDisable(GL_TEXTURE_2D);
		glColor4f(1,1,1,1);

		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
		glEnable(GL_DEPTH_TEST);
	}


}