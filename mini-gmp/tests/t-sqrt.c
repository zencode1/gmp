#include <limits.h>
#include <stdlib.h>
#include <stdio.h>

#include "mini-random.h"

#define MAXBITS 400
#define COUNT 10000

static void
dump (const char *label, const mpz_t x)
{
  char *buf = mpz_get_str (NULL, 16, x);
  fprintf (stderr, "%s: %s\n", label, buf);
  free (buf);
}

/* Called when s is supposed to be floor(sqrt(u)), and r = u - s^2 */
static int
sqrtrem_valid_p (const mpz_t u, const mpz_t s, const mpz_t r)
{
  mpz_t t;

  mpz_init (t);
  mpz_mul (t, s, s);
  mpz_sub (t, u, t);
  if (mpz_sgn (t) < 0 || mpz_cmp (t, r) != 0)
    {
      mpz_clear (t);
      return 0;
    }
  mpz_add_ui (t, s, 1);
  mpz_mul (t, t, t);
  if (mpz_cmp (t, u) <= 0)
    {
      mpz_clear (t);
      return 0;
    }

  mpz_clear (t);
  return 1;
}

int
main (int argc, char **argv)
{
  unsigned i;
  mpz_t u, s, r;

  hex_random_init ();

  mpz_init (u);
  mpz_init (s);
  mpz_init (r);

  for (i = 0; i < COUNT; i++)
    {
      mini_rrandomb (u, MAXBITS);
      mpz_sqrtrem (s, r, u);

      if (!sqrtrem_valid_p (u, s, r))
	{
	  fprintf (stderr, "mpz_sqrtrem failed:\n");
	  dump ("u", u);
	  dump ("sqrt", s);
	  dump ("rem", r);
	  abort ();
	}
    }
  mpz_clear (u);
  mpz_clear (s);
  mpz_clear (r);

  return 0;
}