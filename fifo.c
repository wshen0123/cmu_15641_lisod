#include <stdlib.h>
#include <string.h>
#include "fifo.h"

fifo_t *
fifo_init (ssize_t len_init)
{
  ssize_t size_temp;

  if (len_init > 0)
    size_temp = (len_init / FIFO_BLOCKLEN + 1) * FIFO_BLOCKLEN;
  else
    size_temp = FIFO_BLOCKLEN;

  fifo_t *f = malloc (sizeof (fifo_t));
  if (!f)
    return NULL;

  f->bytes = malloc (size_temp);
  if (!f->bytes)
    {
      free (f);
      return NULL;
    }

  f->size = size_temp;
  f->len = 0;
  return f;
}

char *
fifo_head (fifo_t * fifo)
{
  return fifo->bytes;
}


ssize_t
fifo_len (fifo_t * fifo)
{
  return fifo->len;

}


int
fifo_in (fifo_t * fifo, const char *in_data, ssize_t in_len)
{
  char *bytes_temp;
  ssize_t size_temp;

  if (in_len > 0)
    {
      if (fifo->len + in_len > fifo->size)
	{
	  size_temp =
	    ((fifo->len + in_len) / FIFO_BLOCKLEN + 1) * FIFO_BLOCKLEN;

	  bytes_temp = malloc (size_temp);
	  if (!bytes_temp)
	    return -1;

	  memcpy (bytes_temp, fifo->bytes, fifo->len);
	  memcpy (bytes_temp + fifo->len, in_data, in_len);
	  free (fifo->bytes);

	  fifo->bytes = bytes_temp;
	  fifo->len += in_len;
	  fifo->size = size_temp;

	  return 0;
	}
      else
	{
	  memcpy (fifo->bytes + fifo->len, in_data, in_len);
	  fifo->len += in_len;

	  return 0;
	}
    }
  return 0;
}


void
fifo_out (fifo_t * fifo, ssize_t pop_len)
{
  char *bytes_temp;
  ssize_t size_temp;

  if (pop_len >= fifo->len)
    {
      fifo->len = 0;
      if (fifo->size > FIFO_BLOCKLEN)
	{
	  bytes_temp = malloc (FIFO_BLOCKLEN);
	  if (!fifo->bytes)
	    return;
	  free (fifo->bytes);
	  fifo->bytes = bytes_temp;
	  fifo->size = FIFO_BLOCKLEN;
	}
    }
  else if (pop_len > 0)
    {
      size_temp = ((fifo->len - pop_len) / FIFO_BLOCKLEN + 1) * FIFO_BLOCKLEN;

      if (size_temp < (fifo->size / 2))
	{
	  bytes_temp = malloc (size_temp);
	  if (!bytes_temp)	/* we cannot get more tmp mem, keep it as best */
	    {
	      memcpy (bytes_temp, fifo->bytes + pop_len, fifo->len - pop_len);
	      fifo->len = fifo->len - pop_len;
	    }
	  else
	    {
	      memcpy (bytes_temp, fifo->bytes + pop_len, fifo->len - pop_len);
	      free (fifo->bytes);
	      fifo->bytes = bytes_temp;
	      fifo->len = fifo->len - pop_len;
	      fifo->size = size_temp;
	    }
	}
      else
	{
	  memcpy (fifo->bytes, fifo->bytes + pop_len, fifo->len - pop_len);
	  fifo->len = fifo->len - pop_len;
	}
    }
}


void
fifo_flush (fifo_t * fifo)
{
  fifo_out (fifo, fifo_len (fifo));
}


void
fifo_free (fifo_t * fifo)
{
  if (!fifo)
    return;

  if (fifo->bytes)
    free (fifo->bytes);
  free (fifo);
}
