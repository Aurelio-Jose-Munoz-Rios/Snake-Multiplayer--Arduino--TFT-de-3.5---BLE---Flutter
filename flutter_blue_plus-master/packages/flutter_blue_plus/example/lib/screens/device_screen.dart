import 'dart:async';
import 'package:flutter/material.dart';
import 'package:flutter_blue_plus/flutter_blue_plus.dart';
import '../utils/snackbar.dart';
import '../utils/extra.dart';

class DeviceScreen extends StatefulWidget {
  final BluetoothDevice device;

  const DeviceScreen({super.key, required this.device});

  @override
  State<DeviceScreen> createState() => _DeviceScreenState();
}

class _DeviceScreenState extends State<DeviceScreen> {
  BluetoothConnectionState _connectionState = BluetoothConnectionState.disconnected;
  List<BluetoothService> _services = [];
  bool _isConnecting = false;
  bool _isDisconnecting = false;

  BluetoothCharacteristic? _rxCharacteristic;
  int? _selectedPlayer; // null = no seleccionado, 1 = Jugador 1, 2 = Jugador 2

  late StreamSubscription<BluetoothConnectionState> _connectionStateSubscription;
  late StreamSubscription<bool> _isConnectingSubscription;
  late StreamSubscription<bool> _isDisconnectingSubscription;

  final String characteristicUuidRx = '04d3552e-b9b3-4be6-a8b4-aa43c4507c4d';

  @override
  void initState() {
    super.initState();

    _connectionStateSubscription = widget.device.connectionState.listen((state) async {
      _connectionState = state;
      if (state == BluetoothConnectionState.connected) {
        await _discoverServices();
      }
      if (mounted) {
        setState(() {});
      }
    });

    _isConnectingSubscription = widget.device.isConnecting.listen((value) {
      _isConnecting = value;
      if (mounted) {
        setState(() {});
      }
    });

    _isDisconnectingSubscription = widget.device.isDisconnecting.listen((value) {
      _isDisconnecting = value;
      if (mounted) {
        setState(() {});
      }
    });
  }

  @override
  void dispose() {
    _connectionStateSubscription.cancel();
    _isConnectingSubscription.cancel();
    _isDisconnectingSubscription.cancel();
    super.dispose();
  }

  bool get isConnected {
    return _connectionState == BluetoothConnectionState.connected;
  }

  Future _discoverServices() async {
    try {
      _services = await widget.device.discoverServices();
      for (var service in _services) {
        for (var characteristic in service.characteristics) {
          if (characteristic.uuid.toString() == characteristicUuidRx) {
            _rxCharacteristic = characteristic;
            if (mounted) setState(() {});
            break;
          }
        }
      }
    } catch (e) {
      Snackbar.show(ABC.c, "Error discovering services: $e", success: false);
    }
  }

  Future sendCommand(int command, String label) async {
    if (_rxCharacteristic == null) {
      return;
    }

    try {
      await _rxCharacteristic!.write([command]);
    } catch (e) {
      Snackbar.show(ABC.c, "Error: $e", success: false);
    }
  }

  // Comandos según jugador seleccionado
  Map<String, int> get commands {
    if (_selectedPlayer == 1) {
      return {
        'up': 1,    // Arriba P1
        'down': 2,  // Abajo P1
        'left': 3,  // Izquierda P1
        'right': 4, // Derecha P1
      };
    } else {
      return {
        'up': 11,   // Arriba P2
        'down': 12, // Abajo P2
        'left': 13, // Izquierda P2
        'right': 14, // Derecha P2
      };
    }
  }

  Color get playerColor {
    return _selectedPlayer == 1 ? Colors.green : Colors.yellow;
  }

  Widget buildPlayerSelection() {
    return Center(
      child: Column(
        mainAxisAlignment: MainAxisAlignment.center,
        children: [
          Text(
            'Selecciona tu Jugador',
            style: TextStyle(
              fontSize: 28,
              fontWeight: FontWeight.bold,
              color: Colors.white,
            ),
          ),
          const SizedBox(height: 50),
          
          // Jugador 1
          GestureDetector(
            onTap: () => setState(() => _selectedPlayer = 1),
            child: Container(
              width: 200,
              height: 200,
              decoration: BoxDecoration(
                color: Colors.green.withOpacity(0.2),
                border: Border.all(color: Colors.green, width: 3),
                borderRadius: BorderRadius.circular(20),
                boxShadow: [
                  BoxShadow(
                    color: Colors.green.withOpacity(0.5),
                    blurRadius: 20,
                    spreadRadius: 5,
                  ),
                ],
              ),
              child: Column(
                mainAxisAlignment: MainAxisAlignment.center,
                children: [
                  Icon(Icons.person, size: 80, color: Colors.green),
                  const SizedBox(height: 10),
                  Text(
                    'JUGADOR 1',
                    style: TextStyle(
                      fontSize: 22,
                      fontWeight: FontWeight.bold,
                      color: Colors.green,
                    ),
                  ),
                ],
              ),
            ),
          ),
          
          const SizedBox(height: 30),
          
          // Jugador 2
          GestureDetector(
            onTap: () => setState(() => _selectedPlayer = 2),
            child: Container(
              width: 200,
              height: 200,
              decoration: BoxDecoration(
                color: Colors.yellow.withOpacity(0.2),
                border: Border.all(color: Colors.yellow, width: 3),
                borderRadius: BorderRadius.circular(20),
                boxShadow: [
                  BoxShadow(
                    color: Colors.yellow.withOpacity(0.5),
                    blurRadius: 20,
                    spreadRadius: 5,
                  ),
                ],
              ),
              child: Column(
                mainAxisAlignment: MainAxisAlignment.center,
                children: [
                  Icon(Icons.person, size: 80, color: Colors.yellow),
                  const SizedBox(height: 10),
                  Text(
                    'JUGADOR 2',
                    style: TextStyle(
                      fontSize: 22,
                      fontWeight: FontWeight.bold,
                      color: Colors.yellow,
                    ),
                  ),
                ],
              ),
            ),
          ),
        ],
      ),
    );
  }

  Widget buildDPadButton(IconData icon, int command, String label) {
    return GestureDetector(
      onTap: () => sendCommand(command, label),
      child: Container(
        width: 70,
        height: 70,
        decoration: BoxDecoration(
          color: playerColor.withOpacity(0.3),
          shape: BoxShape.circle,
          border: Border.all(color: playerColor, width: 3),
          boxShadow: [
            BoxShadow(
              color: playerColor.withOpacity(0.5),
              blurRadius: 15,
              spreadRadius: 2,
            ),
          ],
        ),
        child: Icon(icon, size: 35, color: playerColor),
      ),
    );
  }

  Widget buildDPad() {
    return Center(
      child: Column(
        mainAxisAlignment: MainAxisAlignment.center,
        children: [
          // Indicador de jugador
          
          // D-Pad
          Column(
            mainAxisSize: MainAxisSize.min,
            children: [
              buildDPadButton(Icons.arrow_drop_up, commands['up']!, "Arriba"),
              const SizedBox(height: 5),
              Row(
                mainAxisSize: MainAxisSize.min,
                children: [
                  buildDPadButton(Icons.arrow_left, commands['left']!, "Izquierda"),
                  const SizedBox(width: 5),
                  Container(
                    width: 70,
                    height: 70,
                    decoration: BoxDecoration(
                      color: Colors.grey[900],
                      shape: BoxShape.circle,
                      border: Border.all(color: playerColor, width: 2),
                    ),
                    child: Icon(Icons.gamepad, color: playerColor, size: 30),
                  ),
                  const SizedBox(width: 5),
                  buildDPadButton(Icons.arrow_right, commands['right']!, "Derecha"),
                ],
              ),
              const SizedBox(height: 5),
              buildDPadButton(Icons.arrow_drop_down, commands['down']!, "Abajo"),
            ],
          ),
        ],
      ),
    );
  }

  @override
  Widget build(BuildContext context) {
    return ScaffoldMessenger(
      key: Snackbar.snackBarKeyC,
      child: Scaffold(
        backgroundColor: Colors.grey[900],
        body: SafeArea(
          child: isConnected && _rxCharacteristic != null
              ? _selectedPlayer == null
                  ? buildPlayerSelection()
                  : buildDPad()
              : const Center(
                  child: Text(
                    'Conecta el dispositivo primero',
                    style: TextStyle(fontSize: 18, color: Colors.white70),
                  ),
                ),
        ),
      ),
    );
  }
}
