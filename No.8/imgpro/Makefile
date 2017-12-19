PROGS = threshold histimage gradient laplacian template smooth \
	amplify liexpand histplane labeling labeling-stack features mask \
	scale move rotation affine perspect fftimage fftfilter \
	czp sample quantize thinning reverce

PGMS = fruits.pgm image1s.pgm image2s.pgm image3s.pgm kanji.pgm

all: $(PROGS) czp.pgm

amplify: amplify.c image_rw.c params.h
	gcc -O2 -o amplify amplify.c image_rw.c

liexpand: liexpand.c image_rw.c params.h
	gcc -O2 -o liexpand liexpand.c image_rw.c

features: features.c image_rw.c params.h
	gcc -O2 -o features features.c image_rw.c -lm

gradient: gradient.c image_rw.c params.h
	gcc -O2 -o gradient gradient.c image_rw.c -lm

histimage: histimage.c image_rw.c params.h
	gcc -O2 -o histimage histimage.c image_rw.c

histplane: histplane.c image_rw.c params.h
	gcc -O2 -o histplane histplane.c image_rw.c

labeling-stack: labeling-stack.c image_rw.c params.h
	gcc -O2 -o labeling-stack labeling-stack.c image_rw.c

labeling: labeling.c image_rw.c params.h
	gcc -O2 -o labeling labeling.c image_rw.c

laplacian: laplacian.c image_rw.c params.h
	gcc -O2 -o laplacian laplacian.c image_rw.c

mask: mask.c image_rw.c params.h
	gcc -O2 -o mask mask.c image_rw.c

smooth: smooth.c image_rw.c params.h
	gcc -O2 -o smooth smooth.c image_rw.c

template: template.c image_rw.c params.h
	gcc -O2 -o template template.c image_rw.c

thinning: thinning.c image_rw.c params.h
	gcc -O2 -o thinning thinning.c image_rw.c

threshold: threshold.c image_rw.c params.h
	gcc -O2 -o threshold threshold.c image_rw.c

reverce: reverce.c image_rw.c params.h
	gcc -O2 -o reverce reverce.c image_rw.c

scale: scale.c image_rw.c params.h
	gcc -O2 -o scale scale.c image_rw.c

move: move.c image_rw.c params.h
	gcc -O2 -o move move.c image_rw.c

rotation: rotation.c image_rw.c params.h
	gcc -O2 -o rotation rotation.c image_rw.c -lm

affine: affine.c image_rw.c params.h
	gcc -O2 -o affine affine.c image_rw.c -lm

perspect: perspect.c image_rw.c params.h
	gcc -O2 -o perspect perspect.c image_rw.c -lm

fftimage: fftimage.c fft.c image_rw.c params.h
	gcc -O2 -o fftimage fftimage.c image_rw.c fft.c -lm

fftfilter: fftfilter.c fft.c image_rw.c params.h fft.h
	gcc -O2 -o fftfilter fftfilter.c image_rw.c fft.c -lm

czp: czp.c image_rw.c params.h
	gcc -O2 -o czp czp.c image_rw.c -lm

sample: sample.c lpf.c lpf.h image_rw.c params.h
	gcc -O2 -o sample sample.c lpf.c image_rw.c -lm

quantize: quantize.c image_rw.c params.h
	gcc -O2 -o quantize quantize.c image_rw.c -lm

czp.pgm: czp
	./czp 128 128 czp.pgm

clean:
	rm $(PROGS) czp.pgm

install:
	cp $(PROGS) $(HOME)/bin
#	cp $(PGMS) $(HOME)/img
