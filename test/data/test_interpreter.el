; (princ 1999-10-01)
(defun my-test (test another)
  (progn
    (princ "Test is " test "\n" "Another is " another "\n")
    (setq test "TEST!")
    (setq another "ANOTHER!")
    (princ "Test is " test "\n" "Another is " another "\n")
    (setq local-var "local-var")
    (princ "local-var is " local-var "\n")
    )
  )

(my-test "test" "another")
(princ "Test is " test "\n" "Another is " another "\n")
(princ "local-var is " local-var "\n")

(princ (- (+ 1.5 2.5) 1))
