import 'package:flutter/material.dart';

import 'package:birdle/screens/dictionary/root.dart';
import 'package:birdle/screens/game/root.dart';

class Root extends StatelessWidget {
  const Root({super.key});

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      home: DefaultTabController(
        length: 2,
        child: Scaffold(
          appBar: AppBar(
            elevation: 4,
            shadowColor: Colors.black,
            surfaceTintColor: Colors.transparent,
            backgroundColor: Colors.white,
            title: const Align(
              alignment: Alignment.center,
              child: Text(
                "Birdle",
                style: TextStyle(fontWeight: FontWeight.bold),
              ),
            ),
            bottom: const TabBar(
              tabs: [
                Tab(icon: Icon(Icons.sports_esports_outlined)),
                Tab(icon: Icon(Icons.abc_outlined)),
              ],
            ),
          ),
          body: const TabBarView(
            children: [
              Center(child: GameScreen()),
              Center(child: DictionaryScreen()),
            ],
          ),
        ),
      ),
    );
  }
}
