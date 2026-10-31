import 'package:flutter/material.dart';

class Tile extends StatelessWidget {
  const Tile(
    this.letter,
    this.color, {
    super.key,
    this.animate = true,
    this.onClick,
  });

  final String letter;
  final Color color;
  final bool animate;
  final VoidCallback? onClick;

  Widget _AnimatedContainer(BuildContext context, Color color) {
    return AnimatedContainer(
      duration: Duration(milliseconds: 500),
      curve: Curves.bounceIn,
      width: 60,
      height: 60,
      decoration: BoxDecoration(
        border: Border.all(color: Colors.black),
        color: color,
      ),
      child: _square(context),
    );
  }

  Widget _Container(BuildContext context, Color color) {
    return Container(
      width: 60,
      height: 60,
      decoration: BoxDecoration(
        border: Border.all(color: Colors.black),
        color: color,
      ),
      child: _square(context),
    );
  }

  Widget _square(BuildContext context) {
    return Center(
      child: SizedBox(
        width: 60,
        height: 60,
        child: TextButton(
          style: TextButton.styleFrom(
            shape: RoundedRectangleBorder(borderRadius: BorderRadius.zero),
          ),
          onPressed: onClick,
          child: Text(
            letter.toUpperCase(),
            style: Theme.of(context).textTheme.titleLarge,
          ),
        ),
      ),
    );
  }

  @override
  Widget build(BuildContext context) {
    return animate
        ? _AnimatedContainer(context, color)
        : _Container(context, color);
  }
}
