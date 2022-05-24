// Package movingmedian computes the median of a windowed stream of data.
package movingmedian

import (
	// #include "movingmedian.h"
	"C"
)

// MovingMedian computes the moving median of a windowed stream of numbers.
type MovingMedian struct {
	mediator *C.Mediator
}

// NewMovingMedian returns a MovingMedian with the given window size.
func NewMovingMedian(size int) MovingMedian {
	return MovingMedian{C.MediatorNew(C.int(size))}
}

// Push adds an element to the stream, removing old data which has expired from the window.  It runs in O(log windowSize).
func (m *MovingMedian) Push(v float64) {
	C.MediatorInsert(m.mediator, C.double(v))
}

// Median returns the current value of the median from the window.
func (m *MovingMedian) Median() float64 {
	return float64(C.MediatorMedian(m.mediator))
}
