import 'package:flutter/material.dart';

class ResetButton extends StatelessWidget {
  const ResetButton({super.key, required this.onClick});

  final void Function() onClick;

  @override
  Widget build(BuildContext context) {
    return Padding(
      padding: const EdgeInsets.all(8.0),
      child: SizedBox(
        height: 48,
        child: FloatingActionButton.extended(
          icon: Icon(Icons.refresh, color: Colors.black),
          label: Text(
            "Play again",
            style: TextStyle(color: Colors.black, fontWeight: FontWeight.bold),
          ),
          shape: RoundedRectangleBorder(
            borderRadius: BorderRadius.all(Radius.circular(36)),
          ),
          backgroundColor: Colors.white,
          onPressed: onClick,
        ),
      ),
    );
  }
}
