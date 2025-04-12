use core::cmp::PartialEq;
use plotters::prelude::*;
use rand::Rng;
use std::time::Instant;
use std::iter::zip;

fn binsearch<T: PartialEq + PartialOrd>(list: &[T], val: T) -> Option<usize> {
    // Takes a sorted list, and returns the index of an item matching val.
    let mut left: usize = 0;
    let mut right: usize = list.len() - 1;
    let mut midpoint: usize;

    while left <= right {
        midpoint = (left + right) / 2;
        if list[midpoint] == val {
            return Some(midpoint);
        }

        if list[midpoint] > val {
            right = midpoint - 1;
        } else {
            left = midpoint + 1;
        }
    }

    return None;
}

fn linsearch<T: PartialEq>(list: &[T], val: T) -> Option<usize> {
    for i in 0..list.len() {
        if list[i] == val {
            return Some(i);
        }
    }
    return None;
}
fn main() -> Result<(), Box<dyn std::error::Error>> {
    // Plot comparison for various input sizes for a pre-sorted list
    // Plot comparison for various input sizes for an unsorted list pre-sorted list

    let mut rng = rand::rng();

    // We'll average the time over k=samples_per_test samples.
    const SAMPLES_PER_TEST: usize = 3;
    const NUM_TESTS: usize = 10;
    let list_sizes: [usize; NUM_TESTS] =
        [1, 3, 10, 30, 100, 300, 1000, 3000, 10_000, 30_000];

    let mut linsearch_durations: Vec<u128> = vec![0; NUM_TESTS];
    let mut binsearch_durations: Vec<u128> = vec![0; NUM_TESTS];

    // If we assume that the list is unsorted, how much longer does it take to sort and then search
    let mut binsearch_durations_unsorted: Vec<u128> = vec![0; NUM_TESTS];

    for (test_idx, test_size) in list_sizes.iter().enumerate() {
        for _ in 0..SAMPLES_PER_TEST {
            let mut input: Vec<usize> = (0..*test_size)
                .map(|_| rng.random_range(0..*test_size))
                .collect();
            let val_to_find = input[rng.random_range(0..*test_size)];

            let mut timer = Instant::now();
            input.sort();
            binsearch(&input, val_to_find);
            binsearch_durations_unsorted[test_idx] += timer.elapsed().as_nanos();

            timer = Instant::now();
            linsearch(&input, val_to_find);
            linsearch_durations[test_idx] += timer.elapsed().as_nanos();

            timer = Instant::now();
            binsearch(&input, val_to_find);
            binsearch_durations[test_idx] += timer.elapsed().as_nanos();
        }
        linsearch_durations[test_idx] /= SAMPLES_PER_TEST as u128;
        binsearch_durations[test_idx] /= SAMPLES_PER_TEST as u128;
        binsearch_durations_unsorted[test_idx] /= SAMPLES_PER_TEST as u128;
    }

    let mut root = SVGBackend::new("perf-comparison.svg", (800, 800)).into_drawing_area();
    root.fill(&WHITE)?;
    root = root.margin(10, 10, 100, 100);
    

    let mut chart = ChartBuilder::on(&root)
        .x_label_area_size(20)
        .y_label_area_size(20)
        .build_cartesian_2d(
            0..list_sizes[NUM_TESTS - 1],
            0..linsearch_durations[NUM_TESTS - 1],
            // 0..binsearch_durations_unsorted[NUM_TESTS - 1],
        )?;
    chart.configure_mesh().draw()?;
    chart.draw_series(LineSeries::new(zip(list_sizes, linsearch_durations), &RED))?;
    chart.draw_series(LineSeries::new(zip(list_sizes, binsearch_durations), &GREEN))?;
    // chart.draw_series(LineSeries::new(zip(list_sizes, binsearch_durations_unsorted), &BLUE))?;

    root.present()?;
    Ok(())
}
