package com;

import java.util.Comparator;

public class sProcessComparator implements Comparator<sProcess> {
    @Override
    public int compare(sProcess o1, sProcess o2) {
        if (o1.delay == o2.delay) {
            return 0;
        } else if (o1.delay > o2.delay) {
            return 1;
        } else {
            return -1;
        }
    }
}