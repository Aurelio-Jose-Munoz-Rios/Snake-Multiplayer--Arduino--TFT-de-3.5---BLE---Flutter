import 'dart:async';
import 'package:flutter/material.dart';
import 'package:flutter_blue_plus/flutter_blue_plus.dart';

import 'device_screen.dart';
import '../utils/snackbar.dart';
import '../widgets/system_device_tile.dart';
import '../widgets/scan_result_tile.dart';
import '../utils/extra.dart';

class ScanScreen extends StatefulWidget {
  const ScanScreen({super.key});

  @override
  State<ScanScreen> createState() => _ScanScreenState();
}

class _ScanScreenState extends State<ScanScreen> with SingleTickerProviderStateMixin {
  List<BluetoothDevice> _systemDevices = [];
  List<ScanResult> _scanResults = [];
  bool _isScanning = false;
  late StreamSubscription<List<ScanResult>> _scanResultsSubscription;
  late StreamSubscription<bool> _isScanningSubscription;
  late AnimationController _blinkController;

  @override
  void initState() {
    super.initState();

    _blinkController = AnimationController(
      vsync: this,
      duration: const Duration(milliseconds: 800),
    )..repeat(reverse: true);

    _scanResultsSubscription = FlutterBluePlus.scanResults.listen((results) {
      if (mounted) {
        setState(() => _scanResults = results);
      }
    }, onError: (e) {
      Snackbar.show(ABC.b, prettyException("Scan Error:", e), success: false);
    });

    _isScanningSubscription = FlutterBluePlus.isScanning.listen((state) {
      if (mounted) {
        setState(() => _isScanning = state);
      }
    });
  }

  @override
  void dispose() {
    _blinkController.dispose();
    _scanResultsSubscription.cancel();
    _isScanningSubscription.cancel();
    super.dispose();
  }

  Future onScanPressed() async {
    try {
      var withServices = [Guid("180f")];
      _systemDevices = await FlutterBluePlus.systemDevices(withServices);
    } catch (e) {
      Snackbar.show(ABC.b, prettyException("System Devices Error:", e), success: false);
    }
    try {
      await FlutterBluePlus.startScan(
        timeout: const Duration(seconds: 15),
        webOptionalServices: [
          Guid("180f"),
          Guid("180a"),
          Guid("1800"),
          Guid("6e400001-b5a3-f393-e0a9-e50e24dcca9e"),
        ],
      );
    } catch (e) {
      Snackbar.show(ABC.b, prettyException("Start Scan Error:", e), success: false);
    }
    if (mounted) {
      setState(() {});
    }
  }

  Future onStopPressed() async {
    try {
      FlutterBluePlus.stopScan();
    } catch (e) {
      Snackbar.show(ABC.b, prettyException("Stop Scan Error:", e), success: false);
    }
  }

  void onConnectPressed(BluetoothDevice device) {
    device.connectAndUpdateStream().catchError((e) {
      Snackbar.show(ABC.c, prettyException("Connect Error:", e), success: false);
    });
    MaterialPageRoute route = MaterialPageRoute(
      builder: (context) => DeviceScreen(device: device),
      settings: const RouteSettings(name: '/DeviceScreen'),
    );
    Navigator.of(context).push(route);
  }

  Future onRefresh() {
    if (_isScanning == false) {
      FlutterBluePlus.startScan(timeout: const Duration(seconds: 15));
    }
    if (mounted) {
      setState(() {});
    }
    return Future.delayed(const Duration(milliseconds: 500));
  }

  Widget buildArcadeButton() {
    return GestureDetector(
      onTap: _isScanning ? onStopPressed : onScanPressed,
      child: Container(
        padding: const EdgeInsets.symmetric(horizontal: 24, vertical: 12),
        decoration: BoxDecoration(
          color: _isScanning ? Colors.red[700] : Colors.green[600],
          borderRadius: BorderRadius.circular(8),
          border: Border.all(
            color: _isScanning ? Colors.red[300]! : Colors.green[300]!,
            width: 3,
          ),
          boxShadow: [
            BoxShadow(
              color: (_isScanning ? Colors.red : Colors.green).withOpacity(0.5),
              blurRadius: 15,
              spreadRadius: 2,
            ),
          ],
        ),
        child: Row(
          mainAxisSize: MainAxisSize.min,
          children: [
            if (_isScanning) ...[
              FadeTransition(
                opacity: _blinkController,
                child: Icon(Icons.stop_circle, color: Colors.white, size: 20),
              ),
              const SizedBox(width: 8),
              const Text(
                'STOP',
                style: TextStyle(
                  color: Colors.white,
                  fontSize: 16,
                  fontWeight: FontWeight.bold,
                  letterSpacing: 2,
                ),
              ),
            ] else ...[
              const Icon(Icons.search, color: Colors.white, size: 20),
              const SizedBox(width: 8),
              const Text(
                'SCAN',
                style: TextStyle(
                  color: Colors.white,
                  fontSize: 16,
                  fontWeight: FontWeight.bold,
                  letterSpacing: 2,
                ),
              ),
            ],
          ],
        ),
      ),
    );
  }

  Widget buildArcadeDeviceTile(ScanResult result) {
    final deviceName = result.device.platformName.isEmpty 
        ? 'Unknown Device' 
        : result.device.platformName;
    
    return GestureDetector(
      onTap: () => onConnectPressed(result.device),
      child: Container(
        margin: const EdgeInsets.symmetric(horizontal: 16, vertical: 8),
        padding: const EdgeInsets.all(16),
        decoration: BoxDecoration(
          color: Colors.grey[900],
          borderRadius: BorderRadius.circular(12),
          border: Border.all(color: Colors.cyan, width: 2),
          boxShadow: [
            BoxShadow(
              color: Colors.cyan.withOpacity(0.3),
              blurRadius: 10,
              spreadRadius: 1,
            ),
          ],
        ),
        child: Row(
          children: [
            // Icono de Bluetooth parpadeante
            FadeTransition(
              opacity: _blinkController,
              child: Container(
                padding: const EdgeInsets.all(12),
                decoration: BoxDecoration(
                  color: Colors.blue.withOpacity(0.2),
                  shape: BoxShape.circle,
                  border: Border.all(color: Colors.blue, width: 2),
                ),
                child: const Icon(
                  Icons.bluetooth,
                  color: Colors.blue,
                  size: 32,
                ),
              ),
            ),
            const SizedBox(width: 16),
            
            // Info del dispositivo
            Expanded(
              child: Column(
                crossAxisAlignment: CrossAxisAlignment.start,
                children: [
                  Text(
                    deviceName.toUpperCase(),
                    style: const TextStyle(
                      color: Colors.cyan,
                      fontSize: 18,
                      fontWeight: FontWeight.bold,
                      letterSpacing: 1.2,
                    ),
                  ),
                  const SizedBox(height: 4),
                  Text(
                    result.device.remoteId.toString(),
                    style: TextStyle(
                      color: Colors.grey[500],
                      fontSize: 12,
                      fontFamily: 'monospace',
                    ),
                  ),
                  const SizedBox(height: 4),
                  Row(
                    children: [
                      Icon(Icons.signal_cellular_alt, 
                           color: Colors.green[400], size: 16),
                      const SizedBox(width: 4),
                      Text(
                        'RSSI: ${result.rssi} dBm',
                        style: TextStyle(
                          color: Colors.green[400],
                          fontSize: 12,
                          fontWeight: FontWeight.bold,
                        ),
                      ),
                    ],
                  ),
                ],
              ),
            ),
            
            // Flecha de conexión
            Icon(
              Icons.arrow_forward_ios,
              color: Colors.cyan,
              size: 24,
            ),
          ],
        ),
      ),
    );
  }

  @override
  Widget build(BuildContext context) {
    return ScaffoldMessenger(
      key: Snackbar.snackBarKeyB,
      child: Scaffold(
        backgroundColor: Colors.black,
        appBar: AppBar(
          backgroundColor: Colors.grey[900],
          elevation: 0,
          title: Row(
            children: [
              FadeTransition(
                opacity: _blinkController,
                child: const Icon(Icons.games, color: Colors.cyan, size: 28),
              ),
              const SizedBox(width: 12),
              const Text(
                'unicauPy',
                style: TextStyle(
                  color: Colors.cyan,
                  fontSize: 22,
                  fontWeight: FontWeight.bold,
                  letterSpacing: 3,
                ),
              ),
            ],
          ),
          actions: [
            buildArcadeButton(),
            const SizedBox(width: 15),
          ],
          bottom: PreferredSize(
            preferredSize: const Size.fromHeight(4),
            child: Container(
              height: 4,
              decoration: BoxDecoration(
                gradient: LinearGradient(
                  colors: [Colors.cyan, Colors.blue, Colors.purple],
                ),
              ),
            ),
          ),
        ),
        body: RefreshIndicator(
          onRefresh: onRefresh,
          color: Colors.cyan,
          backgroundColor: Colors.grey[900],
          child: _scanResults.isEmpty && !_isScanning
              ? Center(
                  child: Column(
                    mainAxisAlignment: MainAxisAlignment.center,
                    children: [
                      Icon(
                        Icons.bluetooth_searching,
                        size: 100,
                        color: Colors.grey[700],
                      ),
                      const SizedBox(height: 24),
                      Text(
                        'NO DEVICES FOUND',
                        style: TextStyle(
                          color: Colors.grey[600],
                          fontSize: 20,
                          fontWeight: FontWeight.bold,
                          letterSpacing: 2,
                        ),
                      ),
                      const SizedBox(height: 12),
                      Text(
                        'Press SCAN to search',
                        style: TextStyle(
                          color: Colors.grey[700],
                          fontSize: 14,
                        ),
                      ),
                    ],
                  ),
                )
              : ListView(
                  padding: const EdgeInsets.symmetric(vertical: 16),
                  children: [
                    if (_isScanning)
                      Container(
                        margin: const EdgeInsets.all(16),
                        padding: const EdgeInsets.all(20),
                        decoration: BoxDecoration(
                          color: Colors.blue.withOpacity(0.1),
                          borderRadius: BorderRadius.circular(12),
                          border: Border.all(color: Colors.blue, width: 2),
                        ),
                        child: Row(
                          mainAxisAlignment: MainAxisAlignment.center,
                          children: [
                            SizedBox(
                              width: 24,
                              height: 24,
                              child: CircularProgressIndicator(
                                strokeWidth: 3,
                                valueColor: AlwaysStoppedAnimation<Color>(Colors.cyan),
                              ),
                            ),
                            const SizedBox(width: 16),
                            Text(
                              'SCANNING...',
                              style: TextStyle(
                                color: Colors.cyan,
                                fontSize: 18,
                                fontWeight: FontWeight.bold,
                                letterSpacing: 2,
                              ),
                            ),
                          ],
                        ),
                      ),
                    ..._scanResults.map((r) => buildArcadeDeviceTile(r)),
                  ],
                ),
        ),
      ),
    );
  }
}
