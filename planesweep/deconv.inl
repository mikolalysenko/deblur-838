
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
}


