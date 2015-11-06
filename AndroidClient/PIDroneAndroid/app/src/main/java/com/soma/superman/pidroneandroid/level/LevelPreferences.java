package com.soma.superman.pidroneandroid.level;

import android.content.SharedPreferences;
import android.os.Build;
import android.preference.Preference;
import android.preference.PreferenceActivity;

import com.soma.superman.pidroneandroid.level.config.DisplayType;
import com.soma.superman.pidroneandroid.level.config.Viscosity;

/**
 * Created by whee6409 on 15. 11. 5.
 */
public class LevelPreferences extends PreferenceActivity implements Preference.OnPreferenceChangeListener {
    public static final String KEY_SHOW_ANGLE 			= "preference_show_angle";
    public static final String KEY_DISPLAY_TYPE 		= "preference_display_type";
    public static final String KEY_VISCOSITY			= "preference_viscosity";
    public static final String KEY_ECONOMY				= "preference_economy";

    private SharedPreferences prefs;

    @Override
    public boolean onPreferenceChange(Preference preference, Object newValue) {
        String key = preference.getKey();
        if (KEY_DISPLAY_TYPE.equals(key)) {
            CharSequence displaySummary = getText(DisplayType.valueOf((String) newValue).getSummary());
            if (Build.VERSION.SDK_INT >= 11 /* 3.0 : HoneyComb */) {
                displaySummary = String.valueOf(displaySummary).replaceAll("%", "%%");
            }
            preference.setSummary(displaySummary);
        } else if (KEY_VISCOSITY.equals(key)) {
            preference.setSummary(Viscosity.valueOf((String) newValue).getSummary());
        } else if (KEY_ECONOMY.equals(key)) {
            findPreference(KEY_VISCOSITY).setEnabled(!((Boolean) newValue));
        }
        return true;
    }
}
