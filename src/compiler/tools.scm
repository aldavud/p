(require mzlib/pregexp)

(define (string->code s)
    (set! s (pregexp-replace* "_" s "_u_"))
    (set! s (pregexp-replace* "<" s "_lt_"))
    (set! s (pregexp-replace* "\\+" s "_plus_"))
    (set! s (pregexp-replace* "-" s "_min_"))
    (set! s (pregexp-replace* "\\|" s "_bar_"))
    (set! s (pregexp-replace* ">" s "_gt_"))
    (set! s (pregexp-replace* "=" s "_eq_"))
    (set! s (pregexp-replace* "\\?" s "_qm_"))
    (set! s (pregexp-replace* "\\[" s "_lb_"))
    (set! s (pregexp-replace* "\\]" s "_rb_"))
    (set! s (pregexp-replace* "\\{" s "_lbr_"))
    (set! s (pregexp-replace* "\\}" s "_rbr_"))
    (set! s (pregexp-replace* "'" s "_q_"))
    (set! s (pregexp-replace* "\"" s "_dq_"))
    (set! s (pregexp-replace* "\\\\" s "_bs_"))
    (set! s (pregexp-replace* "%" s "_p_"))
    (set! s (pregexp-replace* "\\$" s "_d_"))
    (set! s (pregexp-replace* "#" s "_h_"))
    (set! s (pregexp-replace* "!" s "_ex_"))
    (set! s (pregexp-replace* "@" s "_at_"))
    (set! s (pregexp-replace* "\\^" s "_car_"))
    (set! s (pregexp-replace* "&" s "_amp_"))
    (set! s (pregexp-replace* "\\*" s "_t_"))
    (set! s (pregexp-replace* "/" s "_d_"))
    (set! s (pregexp-replace* ":" s "_col_"))
    (set! s (pregexp-replace* ";" s "_sem_"))
    (set! s (pregexp-replace* "~" s "_til_"))
    (set! s (pregexp-replace* "," s "_com_"))
    (set! s (pregexp-replace* "\\." s "_dot_"))
    s)

(define (list->code input)
  (define (transform segment)
    (cond ((string? segment) segment)
          ((list? segment) (apply string-append (map transform segment)))
          ((symbol? segment) (symbol->string segment))
          ((number? segment) (number->string segment))
          (else (error "Unkown type: " segment))))
  (apply string-append
         (map (lambda (s)
                (string-append (transform s) "\n"))
              input)))

(define (new-collection . values)
    (letrec (
        (add (lambda (value)
            (set! values (cons value values))))
        (self (lambda (msg . args)
            (case msg
                ((add) (apply add args))
                ((values) values)
                (else "Collection DNU: " msg args)))))
        self))

(define (syntax->datum stx)
    (let ((current (syntax-e stx)))
        (if (list? current)
            (map syntax->datum current)
            current)))

(define-syntax syntax-transformer
    (syntax-rules ()
        ((_ name (parser scope stx) ((match ...) toexpand ...) ...)
         (define name
         (let* (
            (stx-transformer (lambda (parser scope stx input)
                (syntax-case input ()
                    ((match ...)
                     (eval #`(begin toexpand ...)))
                    ...
                    (_ (syntax-fail
                            (string-append (symbol->string 'name)
                                           ": bad syntax") stx)))))
            (transformer (lambda (msg . args)
                (unless (eq? msg 'apply)
                    (syntax-fail 
                        (string-append (symbol->string 'name)
                                       ": bad syntax") #'unknown))
                (apply stx-transformer args))))
            transformer)))))
