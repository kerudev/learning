import 'package:flutter/material.dart';

import 'package:birdle/screens/root.dart';
import 'package:birdle/shared/utils/context.dart';

void main() async {
  await initContext();

  runApp(const Root());
}
