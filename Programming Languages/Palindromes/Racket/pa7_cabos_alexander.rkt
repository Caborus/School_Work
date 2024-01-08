#lang racket/base

;pre: integer
;post: boolean showing if the integer is divisible by 4
(define (powerfour n)
  (cond
    [(null? n) #f] ;return f if null
    [(zero? (modulo n 4)) #t] ;return t if divisible by 4
    [else #f] ;if not divisible by 4 return f
  ) ;end conditions
) ;end powerfour


;pre: list, value to be inserted, location in list to insert that value
;post: a new list containing the added value
(define (insert-at lis k n)
  (cond
    [(null? lis) list] ;if list empty, return new list
    [(zero? n) (cons k lis)] ;if insertion point reached, add k to list
    [(cons (car lis) (insert-at (cdr lis) k (- n 1)))] 
      ;create list from current front of list and list returned from insert-at moved one space forward
  ) ;end conditions
) ;end insert-at


;pre: list
;post: the last item within that list
(define (palhelper lis3)
  (cond
    [(null? (cdr lis3)) (car lis3)] ;if the next item in the list is null, then return the current value
    [(palhelper (cdr lis3))] ;if not at the end of the list, check next item
  ) ;end conditions
) ;end palhelper

;pre: list
;post: a 1 if the list is a palendrome (can be read the same forward and backward) and a 0 if not
(define (palendromep lis2)
  (cond
    [(null? lis2) 1] ;if list is null, then it's empty, return a 1
    [(null? (cdr lis2)) 1] ;if there's only 1 item in the list, that's a palendrome, return a 1
    [(eq? (car lis2) (palhelper (cdr lis2))) ;check if first and last characters are equal
     (palendromep (remove (palhelper(cdr lis2)) (cdr lis2)))] ;if they are equal, remove first and last and repeat
    [else 0] ;if all of these fail, not a palendrome, return a 0
  ) ;end conditions
) ;end palendromep

;pre: integer to be checked, current integer 
;post: 1 if prime, 0 if not
(define (primehelper n3 i)
  (cond
    [(eq? n3 i) 1] ;reached the end of numbers to check, is prime, return 1
    [(zero? (modulo n3 i)) 0] ;number is divisible by something other than 1 and itself, return 0
    [else(primehelper n3 (+ i 1))] ;iterate to next value
  ) ;end conditions
) ;end primehelper

;pre: integer
;post: 1 if integer is prime, 0 if it is not
(define (ifprime n2)
  (cond
    ((primehelper n2 2)) ;run primehelper to check all values starting at 2 and ending at n2 - 1
  ) ;end conditions
) ;end ifprime


;start tests
(printf "powerfour 3\n")
(powerfour 3)
(printf "powerfour 12\n")
(powerfour 12)
(printf "insert-at '(1 2 3 4 5) 10 2\n")
(insert-at '(1 2 3 4 5) 10 2)
(printf "palendromep '(r a c e c a r)\n")
(palendromep '(r a c e c a r))
(printf "palendromep '(1 2 3)\n")
(palendromep '(1 2 3))
(printf "ifprime 7\n")
(ifprime 7)
(printf "ifprime 16\n")
(ifprime 16)
;end tests