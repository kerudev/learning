import 'package:flutter/material.dart';

import 'package:birdle/utils/context.dart';
import 'package:birdle/routes/root.dart';

void main() async {
  await initContext();

  runApp(const Root());
}
