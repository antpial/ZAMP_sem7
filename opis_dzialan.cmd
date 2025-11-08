#define ROTATE_SPEED     30
/*
 *  Przykladowy zestaw polecen
 */
  Set   Ob_A 2 0 30 0 0 0      // Polozenie i orientacja obiektu A 
  Set   Ob_B 10 10 0 20 10 0   // Polozenie i orientacja obiektu B
  Rotate Ob_B OX ROTATE_SPEED 40
  Pause  1000                  /* Zawieszenie na 1 sek. */
  Move   Ob_A  10 10
  Rotate Ob_B OZ ROTATE_SPEED 60 /* Gdyby Rotate i Move wykonywane byloby */
  Move   Ob_B 10 20              /* jednoczesnie, to powodowaloby to      */
                                 /* przesuniecie po luku                  */
