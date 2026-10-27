import 'package:flutter/material.dart';

SnackBar Message(
  String message, {
  Duration duration = const Duration(seconds: 3),
}) {
  return SnackBar(
    duration: duration,
    padding: EdgeInsets.zero,
    content: _MessageContent(message: message, duration: duration),
    
  );
}

class _MessageContent extends StatefulWidget {
  final String message;
  final Duration duration;

  const _MessageContent({required this.message, required this.duration});

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
      mainAxisSize: MainAxisSize.min,
      crossAxisAlignment: CrossAxisAlignment.start,
      children: [
        Padding(
          padding: const EdgeInsets.fromLTRB(16, 14, 16, 12),
          child: Text(widget.message),
        ),
        AnimatedBuilder(
          animation: _controller,
          builder: (context, child) {
            return LinearProgressIndicator(
              value: 1 - _controller.value,
              minHeight: 4,
              backgroundColor: Colors.white24,
              valueColor: const AlwaysStoppedAnimation<Color>(
                Colors.tealAccent,
              ),
            );
          },
        ),
      ],
    );
  }
}
