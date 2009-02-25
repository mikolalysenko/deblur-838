
inline void display_progress(int percent_complete)
{
	int ticks = (percent_complete - percent_complete % 2) / 2;

	printf("Progress: [");
	int i = 0;
	for( int i = 0; i < 50; i++)
	{
		if(i == 24)
		{
			printf("%02d%%", percent_complete);
		}
		else if(i < ticks)
		{
			printf("-");
		}
		else
		{
			printf(" ");
		}
	}
	printf("]\r");

	fflush(NULL);
}


template <typename T>
void imDivide(ImageOf<T> &imp, const ImageOf<T> &imq)
{
	int height = imp.Height();
	int width  = imp.Width();
	int nbands = imp.NBands();
	int npixs  = height*width*nbands;

	T* p = imp.FirstPixel();
	const T* q = imq.FirstPixel();
	for(int i = 0; i < npixs; i++)
	{
		*p++ /= *q++;
	}
}


template <typename T>
void imMultiply(ImageOf<T> &imp, const ImageOf<T> &imq)
{
	int height = imp.Height();
	int width  = imp.Width();
	int nbands = imp.NBands();
	int npixs  = height*width*nbands;

	T* p = imp.FirstPixel();
	const T* q = imq.FirstPixel();
	for(int i = 0; i < npixs; i++)
	{
		*p++ *= *q++;
	}
}


template <typename T>
void computeRegularizationDen(const ImageOf<T> &inImg, ImageOf<T> &outImg, const Parameters &params)
{
	int height = inImg.Height();
	int width  = inImg.Width();
	int nbands = inImg.NBands();
	int npixs  = height*width*nbands;

	outImg.ReAllocate(height, width, nbands);
	outImg.Fill((T)0);

	double f, g, Idiff, original, shifted;
	double x2spatialVar = 2.0*params.pSpatialVar;
	double x2intRngVar  = 2.0*params.pIntRngVar;

	for(int ny = -params.pNbrRadius; ny <= params.pNbrRadius; ny++) // for ny = -params.nbrrad:params.nbrrad
	{
		for(int nx = -params.pNbrRadius; nx <= params.pNbrRadius; nx++) // for nx = -params.nbrrad:params.nbrrad
		{
			if(nx == 0 && ny == 0)
			{
				continue;
			}

			f = exp( -(double(ny*ny + nx*nx))/x2spatialVar );

			for(int py = 0; py < height; py++)
			{
				int qy1 = py + ny;
				int qy2 = py - ny;
				if( qy1 < 0 || qy1 >= height || 
					qy2 < 0 || qy2 >= height )   continue;

				for(int px = 0; px < width; px++)
				{
					int qx1 = px + nx;
					int qx2 = px - nx;
					if( qx1 < 0 || qx1 >= width || 
						qx2 < 0 || qx2 >= width )   continue;

					for(int b = 0; b < nbands; b++)
					{
						Idiff = double(inImg.Pixel(py,px,b) - inImg.Pixel(qy1,qx1,b));
						g     = exp(-Idiff*Idiff / x2intRngVar); 
						original = f * g * Idiff / params.pIntRngVar;

						Idiff = double(inImg.Pixel(qy2,qx2,b) - inImg.Pixel(py,px,b));
						g     = exp(-Idiff*Idiff / x2intRngVar);
						shifted  = f * g * Idiff / params.pIntRngVar;

						outImg.Pixel(py,px,b) = outImg.Pixel(py,px,b) + (T)(original - shifted);
					}
				}
			}
		}
	}

	T regfac = (T)params.pRegFactor;
	T* p = outImg.FirstPixel();
	T one = (T)1;
	for(int i = 0; i < npixs; i++)
	{
		*p++ = *p * regfac + one;
	}

/*
    for ny = -params.nbrrad:params.nbrrad 
        
        fprintf('.');
        
        for nx = -params.nbrrad:params.nbrrad
            
            if(nx == 0 && ny == 0)
                continue;
            end
            





            f = exp( -(ny*ny + nx*nx)/(2*params.spavar) );
            
            img = zeros(ih,iw,ib);       
            for py = 1:ih
                qy1 = py + ny;
                qy2 = py - ny;
                if( qy1 < 1 || qy1 > ih || qy2 < 1 || qy2 > ih )
                    continue;
                end
                for px = 1:iw
                    qx1 = px + nx;
                    qx2 = px - nx;
                    if(qx1 < 1 || qx1 > iw || qx2 < 1 || qx2 > iw)
                        continue;
                    end
                    g = zeros(ib,1);
                    Idiff = zeros(ib,1);
                    for b=1:ib
                        Idiff(b) = deblurredImg(py,px,b)- deblurredImg(qy1,qx1,b);
                        g(b)=exp(-Idiff(b)^2 / (2*params.rngvar) );
                    end
                    originl = f*(g.*(Idiff./params.rngvar));
                    
                    g = zeros(ib,1);
                    Idiff = zeros(ib,1);
                    for b=1:ib
                        Idiff(b) = deblurredImg(qy2,qx2,b) - deblurredImg(py,px,b);
                        g(b)=exp(-Idiff(b)^2 / (2*params.rngvar) );
                    end
                    
                    shifted = f*(g.*(Idiff./params.rngvar));
                    
                    img(py,px,:) = originl - shifted;
                end
            end
            
            gradEB = gradEB + img;

        end
    end
*/

}

