import 'package:flutter/material.dart';

import 'package:birdle/shared/utils/context.dart';
import 'package:birdle/screens/root.dart';

void main() async {
  await initContext();

  runApp(const Root());
}
