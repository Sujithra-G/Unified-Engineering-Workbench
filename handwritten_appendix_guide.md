# Two-page handwritten appendix — calculation guide
This is a guide for the candidate to handwrite, sign/identify as appropriate, scan and append. It is not a substitute for a handwritten submission.

Given x=1, y=2, w=0.5, b=0, v=1, c=0:
- z=wx+b=0.5
- h=tanh(z)=0.4621171573
- y_hat=vh+c=0.4621171573
- e=y_hat-y=-1.5378828427
- J=0.5 e^2=1.1825418190

Chain rule:
- dJ/dy_hat=e
- dJ/dv=e h
- dJ/dc=e
- dh/dz=1-h^2
- dJ/dw=e v (1-h^2) x
- dJ/db=e v (1-h^2)

Initial gradients:
- dw=-1.2094644752
- db=-1.2094644752
- dv=-0.7106820475
- dc=-1.5378828427

Simultaneous update with learning rate 0.1:
- w_new=0.6209464475
- b_new=0.1209464475
- v_new=1.0710682047
- c_new=0.1537882843

Explain that each gradient is negative, so subtracting learning_rate × gradient increases the corresponding parameter for this sample and moves the prediction toward target. Handwrite these steps clearly; include the computation tree and update table.
