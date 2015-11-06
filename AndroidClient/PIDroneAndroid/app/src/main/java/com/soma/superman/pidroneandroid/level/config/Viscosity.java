package com.soma.superman.pidroneandroid.level.config;

import com.soma.superman.pidroneandroid.R;

/**
 * Created by whee6409 on 15. 11. 5.
 */
public enum Viscosity {
    HIGH(R.string.viscosity_high_summary, 0.5d),
    MEDIUM(R.string.viscosity_medium_summary, 1d),
    LOW(R.string.viscosity_low_summary, 1.5d);


    private int summary;
    private double coeff;

    private Viscosity(int summary, double coeff) {
        this.summary = summary;
        this.coeff = coeff;
    }

    public double getCoeff() {
        return coeff;
    }

    public int getSummary() {
        return summary;
    }
}
