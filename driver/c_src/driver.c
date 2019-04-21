#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#define MAXTIMINGS 85
#define DHTPIN 7

int dat[5] = {0,0,0,0,0};

void clear_dat()
{
  dat[0] = dat[1] = dat[2] = dat[3] = dat[4] = 0;
}
	     

void read_dht()
{
  uint8_t laststate = HIGH;
  uint8_t counter =0;
  uint8_t j = 0,i;
  float f;

  clear_dat();

  pinMode( DHTPIN, OUTPUT );
  digitalWrite( DHTPIN, LOW );
  delay(20);
  digitalWrite( DHTPIN, HIGH );
  delayMicroseconds( 40 );
  pinMode( DHTPIN, INPUT );

  for ( i = 0; i < MAXTIMINGS; i++ )
    {
      counter = 0;
      while ( digitalRead( DHTPIN ) == laststate )
	{
	  counter++;
	  delayMicroseconds( 1 );
	  if ( counter == 255 )
	    {
	      break;
	    }
	}
      laststate = digitalRead( DHTPIN );
      
      if ( counter == 255 )
	break;
      
      if ( (i >= 4) && (i % 2 == 0) )
	{
	  dat[j / 8] <<= 1;
	  if ( counter > 16 )
	    dat[j / 8] |= 1;
	  j++;
	}
    }
  
  if ( (j >= 40) &&
       (dat[4] == ( (dat[0] + dat[1] + dat[2] + dat[3]) & 0xFF) ) )
    {
      f = dat[2] * 9. / 5. + 32;
      printf( "Humidity = %d.%d %% Temperature = %d.%d C (%.1f F)\n",
	      dat[0], dat[1], dat[2], dat[3], f );
    }else  {
    printf( "Data not good, skip\n" );
  }
}

int main( void )
{
  printf( "Raspberry Pi wiringPi DHT11 Temperature test program\n" );
  
  if ( wiringPiSetup() == -1 )
    exit( 1 );

  while ( 1 )
    {
      read_dht();
      delay( 1000 );
    }

  return(0);
}
