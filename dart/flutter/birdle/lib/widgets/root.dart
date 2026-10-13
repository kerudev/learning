import 'package:flutter/material.dart';

import 'package:birdle/widgets/game_page.dart';

class Root extends StatelessWidget {
  const Root({super.key});

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      home: Scaffold(
        appBar: AppBar(
          title: const Align(
            alignment: Alignment.center,
            child: Text("Birdle"),
          ),
        ),
        body: Center(child: GamePage()),
      ),
    );
  }
}
