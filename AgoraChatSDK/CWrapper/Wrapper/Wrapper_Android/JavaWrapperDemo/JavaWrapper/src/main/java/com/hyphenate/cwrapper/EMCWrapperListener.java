package com.hyphenate.cwrapper;

import com.hyphenate.javawrapper.wrapper.EMWrapperListener;

public class EMCWrapperListener implements EMWrapperListener {
    @Override
    public native void onReceive(String listener, String method, String jsonString);
}