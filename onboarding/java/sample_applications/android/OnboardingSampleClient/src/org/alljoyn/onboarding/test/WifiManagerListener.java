/******************************************************************************
 * Copyright AllSeen Alliance. All rights reserved.
 * <p/>
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 * <p/>
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 ******************************************************************************/

package org.alljoyn.onboarding.test;

import java.util.List;

import android.net.wifi.ScanResult;


/**
 * Interface used to allow the creator of a IskWifiManager to run some code when a network scan is completed.
 */
public interface WifiManagerListener {
    /**
     * This method will be invoked when a network scan is completed.
     *
     * @param results The result of the scan that has completed.
     */
    public void OnScanResultComplete(List<ScanResult> results);
}
