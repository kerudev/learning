import 'package:flutter/material.dart';

import 'package:birdle/utils/game.dart';

class Tile extends StatelessWidget {
  const Tile(this.letter, this.hitType, {super.key, this.onClick});

  final String letter;
  final HitType hitType;
  final void Function()? onClick;

  @override
  Widget build(BuildContext context) {
    return AnimatedContainer(
      duration: Duration(milliseconds: 500),
      curve: Curves.bounceIn,
      width: 60,
      height: 60,
      decoration: BoxDecoration(
        border: Border.all(color: Colors.black),
        color: switch (hitType) {
          HitType.hit => Colors.green,
          HitType.partial => Colors.yellow,
          HitType.miss => Colors.grey,
          _ => Colors.white,
        },
      ),
      child: Center(
        child: TextButton(
          onPressed: onClick,
          child: Text(
            letter.toUpperCase(),
            style: Theme.of(context).textTheme.titleLarge,
          ),
        ),
      ),
    );
  }
}
