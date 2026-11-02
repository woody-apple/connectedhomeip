# Upstream fix: MTRSetupPayload initWithCoder discards CHIP_ERROR (XcodeE -Werror,-Wunused-value)
Toolchain: XcodeE 27E140 (macosx27.5 SDK), via `build --xcode XcodeE`, scheme "Matter Framework Tests" -only-testing MTRSetupPayloadTests (macOS host tests).
Rows:
- RED-build: upstream/master 84f451b52dc, no change -> build FAILS with MTRSetupPayload.mm:681 "-Wunused-value" (expression result unused / ignoring return value of nodiscard) as an error.
- GREEN x2: fix commit -> build succeeds; MTRSetupPayloadTests all pass, including testSecureCodingWithUnparseableQRCode and testSecureCodingRoundtrip.
- MUT-nofallback x2: fix with the fallback removed (err checked and logged, but the per-field decode stays in the else branch, i.e. only when qrCode is nil) -> testSecureCodingWithUnparseableQRCode FAILS (decoded fields nil/0); all other MTRSetupPayloadTests pass.
Each log: line 1 PROOF-SHA, tree, dirty hash; quotes the macosx27.5 SDK line.

## Amendment 1 (before any counted row; GREEN1 attempt = INVALID-infra rsync 255, GREEN2 attempt = config error, neither counted)
XcodeE rejects src/darwin/Framework/Configs/Project.xcconfig MACOSX_DEPLOYMENT_TARGET = 11.0 ("supported 12.0 to 27.5.x"), a separate upstream issue not part of this fix. Every row passes MACOSX_DEPLOYMENT_TARGET=12.0 on the xcodebuild command line, identically for RED, GREEN and MUT. Predictions unchanged.

## Amendment 2 (before any counted test row)
Test step runs on a pool macOS 27.5 (26E181) Mac, session 8c4556f3-8ed9-4812-b887-66a109559f09, udid 00008103-000C78E601FB001E, via `build --device macOS@00008103-000C78E601FB001E` (was: this Mac, macOS 26.6.2 — wrong train; MUT1 and GREEN1 attempts there are INVALID). RED rows are build-only and need no test host. Each test row records the device line. build defaults -collect-test-diagnostics never. Predictions unchanged.

## Amendment 3 (review fix; supersedes the 9bc6324637 rows)
Review comment 1: the test encoded the "qr" key twice (encodeWithCoder: then encodeObject:forKey:@"qr"), relying on undocumented NSKeyedArchiver duplicate-key behaviour. The test now archives through MTRSetupPayloadReplacingArchiver, an NSKeyedArchiver subclass that writes the replacement value for "qr" once. Test-only change; production code identical to 9bc6324637.
New tree => every row re-runs: RED x2 (build fails at MTRSetupPayload.mm:681 only), GREEN x2 (MTRSetupPayloadTests 26 executed, 0 failures), MUT x2 (26 executed, 4 failures, all in testSecureCodingWithUnparseableQRCode). Same host rules as amendment 2 (pool macOS 27.5 26E181; MACOSX_DEPLOYMENT_TARGET=12.0). Predictions unchanged.

## Amendment 4 (before any v2 test row)
v2 test host: pool macOS 27.5 (26E181) Mac, session 165e2aa8-aaaa-40eb-9a91-eb3aec9f8003, udid 00008103-0015459601FB001E (lease 4; first device failed install on low battery, this one's initial attach hit the CoreDevice init timeout and attached on one retry). v2 trees: GREEN 77c3bf8e5f / 8b34911e1c, MUT 1e61544a87 / 906a9758ee. Predictions unchanged.

## Amendment 5 (v3: concatenated-payload guard; written before any v3 row)
Review thread (CodeRabbit, MTRSetupPayload.mm:688): a concatenated payload's sub-payloads are archived only in its QR code; when that QR fails to parse, the v2 fallback decodes the first component's fields and, since `concatenated` is never archived, also decodes hasShortDiscriminator/discriminator/rendezvousInformation as if stand-alone. v3: encodeWithCoder: writes MTRSP.ck.concatenated=YES for concatenated payloads; initWithCoder: returns nil when the QR fails and that flag is set. Archives without the flag (older archives, stand-alone payloads) keep the v2 fallback.
These rows are ADDITIVE: production changes, so every row re-runs on the v3 trees, but the v2 rows stay valid evidence for the stand-alone fallback at a3f70eaea2.
New test: testSecureCodingConcatenatedWithUnparseableQRCode (concatenated "MT:M5L90MP500K64J00000*M5L90MP500OC8010000", qr replaced with an unparseable string; asserts initWithCoder: returns nil).
Trees: GREEN v3 4d39996896 / ecc7640b6f. RED c9df7d4329 / fd783353ab (master 84f451b52d + v3 tests). RED-concat b0390774cf / c351e16285 (PR head a3f70eaea2 + v3 tests). MUT-nofb 26cbff2e42 / d2c386b1f6 (fallback condition back to qrCode == nil). MUT-noguard 9c16d79b6e / 8ba1c26fc8 (nil-return guard removed). MUT-noflag 141b0f3547 / d8c88b000b (flag no longer encoded).
Host and flags as amendment 4 (Mac session 165e2aa8, udid 00008103-0015459601FB001E, macOS 27.5 26E181, XcodeE, MACOSX_DEPLOYMENT_TARGET=12.0, -only-testing MatterTests/MTRSetupPayloadTests).
Predictions (each x2):
- RED: build fails with exactly one error, MTRSetupPayload.mm:681:9 nodiscard ChipError [-Werror,-Wunused-value]; no tests run.
- RED-concat: 27 executed, 1 failure: testSecureCodingConcatenatedWithUnparseableQRCode, XCTAssertNil fails (decoded non-nil). All others pass.
- GREEN: 27 executed, 0 failures.
- MUT-nofb: 27 executed, 4 failures, all in testSecureCodingWithUnparseableQRCode (vendorID, productID, setupPasscode, serialNumber); the concatenated test passes (the guard returns nil before the fallback).
- MUT-noguard: 27 executed, 1 failure: testSecureCodingConcatenatedWithUnparseableQRCode XCTAssertNil.
- MUT-noflag: 27 executed, 1 failure: testSecureCodingConcatenatedWithUnparseableQRCode XCTAssertNil.

## Amendment 6 (v4: concatenation detected from the archived QR code; written before any v4 row)
Gate review (Idunn) on v3: the MTRSP.ck.concatenated flag protects only archives written by v3. A concatenated payload archived by an earlier version has no flag, so an unparseable QR still fell back to a stand-alone payload built from the first component. v4 drops the flag. On a QR decode failure, initWithCoder: returns nil when the archived QR code contains chip::kPayloadDelimiter ('*'), which is outside the Base38 alphabet (src/setup_payload/Base38.h kCodes), so it appears only in concatenated QR codes (QRCodeSetupPayloadParser::populatePayloads splits on it). Stand-alone payloads, and archives without a QR code, keep the fallback. The v2 and v3 rows stay valid for their trees; every v4 row re-runs.
Test changes: testSecureCodingConcatenatedWithUnparseableQRCode replaces "qr" with "MT:not a setup payload*nor is this" (no new key, so it is the shape a pre-v3 archive has), asserts initWithCoder: returns nil, and asserts +[NSKeyedUnarchiver unarchivedObjectOfClass:fromData:error:] returns nil (the caller-visible result).
Trees: GREEN v4 62be4579e5 / 20099c53c1. RED 1738828dc9 / eb99d3fdbe (master + v4 tests). RED-concat eb2f7a307a / 61bc557777 (PR head a3f70eaea2 + v4 tests). MUT-nofb bef02a3500 / 36ea97eff6 (fallback condition back to qrCode == nil). MUT-noguard 9e62a66b2c / 1c7d7c7e7e (delimiter guard removed). No MUT-noflag: v4 has no flag.
Host and flags as amendment 4 (Mac session 165e2aa8, udid 00008103-0015459601FB001E, macOS 27.5 26E181, XcodeE, MACOSX_DEPLOYMENT_TARGET=12.0, -only-testing MatterTests/MTRSetupPayloadTests).
Predictions (each x2):
- RED: build fails with exactly one error, MTRSetupPayload.mm:681:9 nodiscard ChipError [-Werror,-Wunused-value]; no tests run.
- RED-concat: 27 executed, failures only in testSecureCodingConcatenatedWithUnparseableQRCode (both XCTAssertNil, decoded non-nil). All others pass.
- GREEN: 27 executed, 0 failures.
- MUT-nofb: 27 executed, 4 failures, all in testSecureCodingWithUnparseableQRCode (vendorID, productID, setupPasscode, serialNumber); the concatenated test passes.
- MUT-noguard: 27 executed, failures only in testSecureCodingConcatenatedWithUnparseableQRCode (both XCTAssertNil).

## Amendment 7 (v5: test-only fix to the caller-level assertion; written before any v5 row)
The v4 rows (logged, not counted for the final tree) showed the second XCTAssertNil in testSecureCodingConcatenatedWithUnparseableQRCode never failed, not even on RED-concat or MUT-noguard. Cause: it decoded archiver.encodedData, which holds the payload's keys at the top level with no NSKeyedArchiveRootObjectKey, so +unarchivedObjectOfClass:fromData:error: returned nil for every tree. Only the first assertion (line 516, initWithCoder:) carried the result. v5 archives the payload as the root object through a second MTRSetupPayloadReplacingArchiver with the same "qr" replacement, so that assertion now depends on initWithCoder:. Production code is identical to v4 (git diff 62be4579e5 v5 -- src/darwin/Framework/CHIP is empty).
Trees: GREEN v5 57a2a17e47 / 536c09e56b. RED 87d388de0f / 8e8e7b1c91. RED-concat 6d9b935745 / 6c1d7cb5b5 (PR head a3f70eaea2 + v5 tests). MUT-nofb bb552567c5 / ca986d676e. MUT-noguard 81aae5f997 / 243acce6cb.
Host and flags as amendment 6.
Predictions (each x2):
- RED: build fails with exactly one error, MTRSetupPayload.mm:681:9; no tests run.
- RED-concat: 27 executed, 2 failures, both in testSecureCodingConcatenatedWithUnparseableQRCode (the initWithCoder: XCTAssertNil and the unarchivedObjectOfClass: XCTAssertNil).
- GREEN: 27 executed, 0 failures.
- MUT-nofb: 27 executed, 4 failures, all in testSecureCodingWithUnparseableQRCode; the concatenated test passes.
- MUT-noguard: 27 executed, 2 failures, both in testSecureCodingConcatenatedWithUnparseableQRCode (the same two assertions).
