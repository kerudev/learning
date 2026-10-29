import 'package:flutter/material.dart';

SnackBar Message(
  String message, {
  Duration duration = const Duration(seconds: 3),
  Color color = Colors.tealAccent,
}) {
  return SnackBar(
    content: _MessageContent(
      message: message,
      duration: duration,
      color: color,
    ),
    duration: duration,
    elevation: 2,
    width: 400,
    padding: EdgeInsets.zero,
    backgroundColor: Colors.white,
    behavior: SnackBarBehavior.floating,
    showCloseIcon: true,
    closeIconColor: Colors.black,
    shape: RoundedRectangleBorder(
      borderRadius: BorderRadius.all(Radius.circular(24)),
    ),
  );
}

class _MessageContent extends StatefulWidget {
  final String message;
  final Duration duration;
  final Color color;

  const _MessageContent({
    required this.message,
    required this.duration,
    required this.color,
  });

  @override
  State<_MessageContent> createState() => _MessageContentState();
}

class _MessageContentState extends State<_MessageContent>
    with SingleTickerProviderStateMixin {
  late final AnimationController _controller;

  @override
  void initState() {
    super.initState();

    _controller = AnimationController(vsync: this, duration: widget.duration)
      ..forward();
  }

  @override
  void dispose() {
    _controller.dispose();
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    return Column(
      children: [
        Padding(
          padding: const EdgeInsets.fromLTRB(16, 14, 16, 12),
          child: Text(
            widget.message,
            style: TextStyle(color: Colors.black, fontWeight: FontWeight.bold),
          ),
        ),
        Padding(
          padding: EdgeInsetsGeometry.directional(start: 40),
          child: AnimatedBuilder(
            animation: _controller,
            builder: (context, child) => LinearProgressIndicator(
              value: 1 - _controller.value,
              minHeight: 4,
              backgroundColor: Colors.white,
              valueColor: AlwaysStoppedAnimation<Color>(widget.color),
              borderRadius: BorderRadius.all(Radius.circular(24)),
            ),
          ),
        ),
      ],
    );
  }
}
